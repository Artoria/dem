#include <cstdio>
#include <cstdlib>

struct dem_node{
  long klass;
  union {
    unsigned long value;
    unsigned long object;
    int int_value;
    float float_value;
  };
};

enum {
  CLASS_INT,
  CLASS_FLOAT
};

struct dem_stack{
  int size;
  int top;
  dem_node buf[];
};


extern "C"{
dem_stack* dem_new(int size){
  if (size == -1) { size = 1024; } 
  dem_stack *x = (dem_stack*)malloc(sizeof(dem_stack) + sizeof(dem_node[size]));
  x->size = size;
  x->top  = 0;
  return x;
}

void dem_free(dem_stack *x){
  free(x);
}

int dem_push_int(dem_stack *d, int k){
  if (d->top + 1 < d -> size){
    d->buf[++d->top].klass = CLASS_INT;
    d->buf[d->top].int_value = k;
    return 1;
  }else{
    return 0;
  }
}

int dem_push_float(dem_stack *d, float k){
  if (d->top + 1 < d -> size){
    d->buf[++d->top].klass = CLASS_FLOAT;
    d->buf[d->top].float_value = k;
    return 1;
  }else{
    return 0;
  }
}

int dem_push_object(dem_stack *d, void *klass, void *object){
  if (d->top + 1 < d -> size){
    d->buf[++d->top].klass   = (long)klass;
    d->buf[d->top].value = (unsigned long)object;
    return 1;
  }else{
    return 0;
  }
}

int dem_push_ary(dem_stack *d, dem_node *array, int len){
  if (d->top + 1 < d -> size){
    d->buf[++d->top].klass = -len;
    d->buf[d->top].value   = (unsigned long)array;
    return 1;
  }else{
    return 0;
  }
}


int dem_push(dem_stack *d, dem_node node){
  if (d->top + 1 < d -> size){
    d->buf[++d->top] = node;
    return 1;
  }else{
    return 0;
  }
}


int dem_pop(dem_stack *d, dem_node *out){
  if (d->top >= 0){
     *out = d->buf[d->top--];
     return 1;
  }  else {
     return 0;
  }
}

typedef void (*dem_stack_function)(unsigned long object, dem_stack *stack);
typedef dem_node (*dem_function)(unsigned long object);

typedef void (__stdcall *dem_stack_function_stdcall)(unsigned long object, dem_stack *stack);
typedef dem_node (__stdcall *dem_function_stdcall)(unsigned long object);


void dem_call(dem_stack *d){
  dem_node object;
  dem_pop(d, &object);
  dem_function dd = (dem_function)object.klass;
  dem_node result = dd(object.object);
  dem_push(d, result);
}

void dem_call_stack(dem_stack *d){
  dem_node object;
  dem_pop(d, &object);
  dem_stack_function dd = (dem_stack_function)object.klass;
  dd(object.object, d);
}

void dem_call_stdcall(dem_stack *d){
  dem_node object;
  dem_pop(d, &object);
  dem_function_stdcall dd = (dem_function_stdcall)object.klass;
  dem_node result = dd(object.object);
  dem_push(d, result);
}

void dem_call_stack_stdcall(dem_stack *d){
  dem_node object;
  dem_pop(d, &object);
  dem_stack_function_stdcall dd = (dem_stack_function_stdcall)object.klass;
  dd(object.object, d);
}


}

/*
void add(unsigned long object, dem_stack *stack){
  dem_node a, b;
  dem_pop(stack, &a);
  dem_pop(stack, &b);
  dem_push_int(stack, a.int_value + b.int_value);
}

void print(unsigned long object, dem_stack *stack){
  dem_node a;
  dem_pop(stack, &a);
  printf("%d\n", a.int_value);
}


int main(){
  dem_stack *d = dem_new(1024);
  dem_push_int(d, 3);
  dem_push_int(d, 5);
  dem_push_object(d, (void *)add, 0);
  dem_call_stack(d);
  dem_push_object(d, (void *)print, 0);
  dem_call_stack(d);
  dem_free(d);
}
*/