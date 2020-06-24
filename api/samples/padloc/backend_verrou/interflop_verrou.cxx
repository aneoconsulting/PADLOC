
/*--------------------------------------------------------------------*/
/*--- Verrou: a FPU instrumentation tool.                          ---*/
/*--- Interface for floating-point operations overloading.         ---*/
/*---                                                 vr_fpOps.cxx ---*/
/*--------------------------------------------------------------------*/

/*
   This file is part of Verrou, a FPU instrumentation tool.

   Copyright (C) 2014-2016
     F. Févotte     <francois.fevotte@edf.fr>
     B. Lathuilière <bruno.lathuiliere@edf.fr>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA.

   The GNU General Public License is contained in the file COPYING.
*/

#include "interflop_verrou.h"
#include "vr_nextUlp.hxx"
#include "vr_isNan.hxx"
#include "vr_fma.hxx"
#include <stddef.h>
//extern "C" {
#include "vr_rand.h"
//}

#include <iostream>
#include <ctime>


#include "vr_roundingOp.hxx"
#include "vr_op.hxx"
#include "interflop_backend_interface.h"


// * Global variables & parameters
int CHECK_C  = 0;
vr_RoundingMode DEFAULTROUNDINGMODE;
vr_RoundingMode ROUNDINGMODE;
unsigned int vr_seed;
void (*vr_panicHandler)(const char*)=NULL;
void (*vr_nanHandler)()=NULL;



void verrou_set_panic_handler(void (*panicHandler)(const char*)){
  vr_panicHandler=panicHandler;
}

void verrou_set_nan_handler(void (*nanHandler)()){
  vr_nanHandler=nanHandler;
}


void (*vr_debug_print_op)(int,const char*, const double*, const double*)=NULL;
void verrou_set_debug_print_op(void (*printOpHandler)(int nbArg,const char*name, const double* args,const double* res)){
  vr_debug_print_op=printOpHandler;
};


// * Operation implementation
const char*  verrou_rounding_mode_name (enum vr_RoundingMode mode) {
  switch (mode) {
  case VR_NEAREST:
    return "NEAREST";
  case VR_UPWARD:
    return "UPWARD";
  case VR_DOWNWARD:
    return "DOWNWARD";
  case VR_ZERO:
    return "TOWARD_ZERO";
  case VR_RANDOM:
    return "RANDOM";
  case VR_AVERAGE:
    return "AVERAGE";
  case VR_FARTHEST:
    return "FARTHEST";
  case VR_FLOAT:
    return "FLOAT";
  case VR_NATIVE:
    return "NATIVE";
  }

  return "undefined";
}




// * C interface
void IFV_FCTNAME(configure)(vr_RoundingMode mode,void* context) {
  verrou_set_random_seed();
  DEFAULTROUNDINGMODE = mode;
  ROUNDINGMODE=mode;
}

void IFV_FCTNAME(finalyze)(void* context){
}

const char* IFV_FCTNAME(get_backend_name)() {
  return "verrou";
}

const char* IFV_FCTNAME(get_backend_version)() {
  return "1.x-dev";
}

void verrou_begin_instr(){
  ROUNDINGMODE=DEFAULTROUNDINGMODE;
}

void verrou_end_instr(){
  ROUNDINGMODE= VR_NEAREST;
}

void verrou_set_seed (unsigned int seed) {
  vr_seed = vr_rand_int (&vr_rand);
  vr_rand_setSeed (&vr_rand, seed);
}

void verrou_set_random_seed () {
  std::srand(std::time(nullptr)); 
  verrou_set_seed(std::rand());
  //vr_rand_setSeed(&vr_rand, vr_seed);
}

void IFV_FCTNAME(add_double) (double a, double b, double* res,void* context) {
  typedef OpWithSelectedRoundingMode<AddOp <double>,double > Op;
  Op::apply(Op::PackArgs(a,b),res,context);
}

void IFV_FCTNAME(add_float) (float a, float b, float* res,void* context) {
  typedef OpWithSelectedRoundingMode<AddOp <float>,float > Op;
  Op::apply(Op::PackArgs(a,b),res,context);
}

void IFV_FCTNAME(sub_double) (double a, double b, double* res,void* context) {
  typedef OpWithSelectedRoundingMode<SubOp <double>,double > Op;
  Op::apply(Op::PackArgs(a,b),res,context);
}

void IFV_FCTNAME(sub_float) (float a, float b, float* res,void* context) {
  typedef OpWithSelectedRoundingMode<SubOp <float>,float > Op;
  Op::apply(Op::PackArgs(a,b),res,context);
}

void IFV_FCTNAME(mul_double) (double a, double b, double* res,void* context) {
  typedef OpWithSelectedRoundingMode<MulOp <double> ,double> Op;
  Op::apply(Op::PackArgs(a,b),res,context);
}

void IFV_FCTNAME(mul_float) (float a, float b, float* res,void* context) {
  typedef OpWithSelectedRoundingMode<MulOp <float>,float > Op;
  Op::apply(Op::PackArgs(a,b),res,context);
}

void IFV_FCTNAME(div_double) (double a, double b, double* res,void* context) {
  typedef OpWithSelectedRoundingMode<DivOp <double>,double > Op;
  Op::apply(Op::PackArgs(a,b),res,context);
}

void IFV_FCTNAME(div_float) (float a, float b, float* res,void* context) {
  typedef OpWithSelectedRoundingMode<DivOp <float>,float > Op;
  Op::apply(Op::PackArgs(a,b),res,context);
}

void IFV_FCTNAME(cast_double_to_float) (double a, float* res, void* context){
  typedef OpWithSelectedRoundingMode<CastOp<double,float>,float > Op;
  Op::apply(Op::PackArgs(a),res,context);
}

void IFV_FCTNAME(madd_double) (double a, double b, double c, double* res, void* context){
  typedef OpWithSelectedRoundingMode<MAddOp <double>,double > Op;
  Op::apply(Op::PackArgs(a,b,c), res,context);
}

void IFV_FCTNAME(madd_float) (float a, float b, float c, float* res, void* context){
  typedef OpWithSelectedRoundingMode<MAddOp <float>,float > Op;
  Op::apply(Op::PackArgs(a,b,c), res, context);
}




struct interflop_backend_interface_t IFV_FCTNAME(init)(void ** context){
  struct interflop_backend_interface_t config;

  config.interflop_add_float = & IFV_FCTNAME(add_float);
  config.interflop_sub_float = & IFV_FCTNAME(sub_float);
  config.interflop_mul_float = & IFV_FCTNAME(mul_float);
  config.interflop_div_float = & IFV_FCTNAME(div_float);

  config.interflop_add_double = & IFV_FCTNAME(add_double);
  config.interflop_sub_double = & IFV_FCTNAME(sub_double);
  config.interflop_mul_double = & IFV_FCTNAME(mul_double);
  config.interflop_div_double = & IFV_FCTNAME(div_double);

  config.interflop_cast_double_to_float=& IFV_FCTNAME(cast_double_to_float);

  config.interflop_madd_float = & IFV_FCTNAME(madd_float);
  config.interflop_madd_double =& IFV_FCTNAME(madd_double);

  return config;
}
