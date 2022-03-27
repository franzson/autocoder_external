#include "ext.h"							// standard Max include, always required
#include "ext_obex.h"						// required for new style Max object
#include <time.h>
#include "c_api.h"
#include <math.h>




typedef struct _autocoder
{
    t_object					ob;			// the object itself (must be first)
    void*                       list_out;
    float                       input[8];
    float                       *output;
    float                       *z_1;
    
    
    float                       z_feedback;
    float                       brightness;
    float                       whitening;
    t_atom                      *out;
    TfLiteModel*                model;
    TfLiteInterpreter*          interpreter;
    TfLiteInterpreterOptions*   options;
    float                       scale_mult[8];
    float                       scale_subtract[8];
    float                       other_values[8];
    bool                        loaded;
    long                        output_size;
} t_autocoder;

void *autocoder_new(t_symbol *s, long argc, t_atom *argv);
void autocoder_free(t_autocoder *x);
void autocoder_anything(t_autocoder *x, t_symbol *s, long argc, t_atom *argv);
void autocoder_assist(t_autocoder *x, void *b, long m, long a, char *s);
void *autocoder_class;
float clamp(float d, float min, float max);
void load(t_autocoder *x, const char *filename_model, const char *filename_mm);
void decode(t_autocoder *x);

float clamp(float d, float min, float max) {
    const float t = d < min ? min : d;
    return t > max ? max : t;
}

void ext_main(void *r){
    t_class *c;
    
    c = class_new("autocoder", (method)autocoder_new, (method)autocoder_free, (long)sizeof(t_autocoder),
                  0L /* leave NULL!! */, A_GIMME, 0);
    
    class_addmethod(c, (method)autocoder_assist,			"assist",		A_CANT,     0);
    class_addmethod(c, (method)autocoder_anything,          "anything",     A_GIMME,    0);
    class_register(CLASS_BOX, c); /* CLASS_NOBOX */
    autocoder_class = c;
}

void load(t_autocoder *x, const char *filename_model, const char *filename_mm){
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_num = 0;
    
    fp = fopen(filename_mm, "r");
    if(fp!=NULL)
        while((read = getline(&line, &len, fp)) != -1) {
            char * token = strtok(line, " ");
            int p = 0;
            while( token != NULL ) {
                if(line_num == 0){
                    x->scale_mult[p] = atof(token);
                } else if( line_num == 1) {
                    x->scale_subtract[p] = atof(token);
                } else if( line_num == 2) {
                    x->other_values[p] = atof(token);
                }
                token = strtok(NULL, " ");
                p++;
            }
            line_num++;
        }
    
    fclose(fp);
    if(line)
        free(line);
    
    x->model = TfLiteModelCreateFromFile(filename_model);
    x->options = TfLiteInterpreterOptionsCreate();
    
    
    x->interpreter = TfLiteInterpreterCreate(x->model, x->options);
    TfLiteInterpreterOptionsSetNumThreads(x->options, 4);

    TfLiteInterpreterAllocateTensors(x->interpreter);
    x->loaded = TRUE;
    
    post("... done loading");
     
}

void decode(t_autocoder *x){
    if(x->loaded){

        for (int i = 0; i < 8; i++){
            x->input[i] = x->input[i] * x->scale_mult[i] + x->scale_subtract[i];
        }
        
        TfLiteTensor* input_tensor = TfLiteInterpreterGetInputTensor(x->interpreter, 0);
        TfLiteTensorCopyFromBuffer(input_tensor, x->input, 8 * sizeof(float));
        TfLiteInterpreterInvoke(x->interpreter);
        
        const TfLiteTensor* output_tensor = TfLiteInterpreterGetOutputTensor(x->interpreter, 0);
        TfLiteTensorCopyToBuffer(output_tensor, x->output, x->output_size * sizeof(float));
        
        
        /*  SMEAR  */
        for(int i = 0; i < x->output_size; i++){
            x->output[i] *= (1. - x->z_feedback);
            x->z_1[i] *= x->z_feedback;
            x->output[i] += x->z_1[i];
            x->z_1[i] = x->output[i];
        }
        
        /*  BRIGHTEN  */
        for(int i = 0; i < x->output_size; i++){
            x->output[i] *= pow(i/512., x->brightness);
        }
        
        /*  WHITEN  */
        for(int i = 0; i < x->output_size; i++){
            x->output[i] = pow(x->output[i], x->whitening) * x->other_values[1]  / 32.;
        }

        
        atom_setfloat_array(x->output_size, x->out, x->output_size, x->output);
        outlet_list(x->list_out, NULL, x->output_size, x->out);
       

    }
}

void autocoder_assist(t_autocoder *x, void *b, long m, long a, char *s){
    if (m == ASSIST_INLET) { // inlet
        sprintf(s, "/open modelpath /decode 8 values between 0 and 1");
    }
    else {	// outlet
        sprintf(s, "returns 512 mel values");
    }
}

void autocoder_free(t_autocoder *x){

    TfLiteInterpreterDelete(x->interpreter);
    TfLiteInterpreterOptionsDelete(x->options);
    TfLiteModelDelete(x->model);

    if (x->output) {
        free(x->output);
        x->output = NULL;
    }
    if (x->z_1) {
        free(x->z_1);
        x->z_1 = NULL;
    }
    if (x->out) {
        free(x->out);
        x->out = NULL;
    }
}



void *autocoder_new(t_symbol *s, long argc, t_atom *argv){
    t_autocoder *x = NULL;
    long i;
    
    if ((x = (t_autocoder *)object_alloc(autocoder_class))) {
        x->list_out = listout(x);
        x->loaded = FALSE;
        x->output_size = 512;
        x->output = malloc(sizeof(float) * x->output_size);
        x->z_1 = malloc(sizeof(float) * x->output_size);
        x->out = malloc(sizeof(t_atom) * x->output_size);
    }

    post("Tensorflow version %s", TfLiteVersion());
    post("autocoder version 0.12");
    
    x->z_feedback = 0.;
    x->brightness = .001;
    x->whitening = 1.;
        
    return (x);
}

void autocoder_anything(t_autocoder *x, t_symbol *s, long argc, t_atom *argv){
    t_atom *ap;
    
    if (s == gensym("/open")) {
        ap = argv;
        post("... loading model %s\n", atom_getsym(ap)->s_name);
        char filename_[1024];
        char filename_mm[1024];
        char filename_model[1024];
        strcpy(filename_mm, atom_getsym(ap)->s_name);
        strcpy(filename_model, atom_getsym(ap)->s_name);
        strcat(filename_mm, ".mm");
        strcat(filename_model, ".dec");
        load(x, filename_model, filename_mm);
    } else if (s == gensym("/open/fft")) {
        ap = argv;
        post("... loading model %s\n", atom_getsym(ap)->s_name);
        char filename_[1024];
        char filename_mm[1024];
        char filename_model[1024];
        strcpy(filename_mm, atom_getsym(ap)->s_name);
        strcpy(filename_model, atom_getsym(ap)->s_name);
        strcat(filename_mm, ".mm");
        strcat(filename_model, ".fft.dec");
        load(x, filename_model, filename_mm);
    } else if (s == gensym("/decode")) {
        ap = argv;
        if (argc > 8){
            argc = 8;
        }
        atom_getfloat_array(8, ap, 8, x->input);
        decode(x);
    } else if (s == gensym("/feedback")) {
        x->z_feedback = clamp(atom_getfloat(argv), 0., .999);
    } else if (s == gensym("/brightness")) {
        x->brightness = clamp(atom_getfloat(argv), .001, 2.);
    } else if (s == gensym("/whitening")) {
        x->whitening = clamp(atom_getfloat(argv), .001, 2.);
    } else if (s == gensym("/output_size")) {
        x->output_size = atom_getlong(argv);
        free(x->output);
        free(x->z_1);
        free(x->out);
        x->output = malloc(sizeof(float) * x->output_size);
        x->z_1 = malloc(sizeof(float) * x->output_size);
        x->out = malloc(sizeof(t_atom) * x->output_size);

        post("... setting output size to %ld\n", x->output_size);
    }
}


