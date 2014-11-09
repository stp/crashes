#include <stdio.h>
#include "c_interface.h"

int main() {
  VC vc = vc_createValidityChecker();

  // Numbers will be non-negatives integers bounded at 2^32                                              
  Type bv32 = vc_bvType(vc, 32);

  // Determine whether the following equations are satisfiable:                                          
  //   v + 4 = n                                                                                         
  //   4 = n                                                                                             

  // Construct variable n                                                                                
  Expr n = vc_varExpr(vc, "n", bv32);

  // Construct v + 4                                                                                     
  Expr v = vc_varExpr(vc, "v", bv32);
  Expr ct_4 = vc_bvConstExprFromInt(vc, 32, 4);
  Expr add_v_4 = vc_bvPlusExpr(vc, 32, v, ct_4);

  // Because numbers are represented as bit vectors,                                                     
  // addition can roll over.  So construct a constraint                                                  
  // expresses that v+4 does not overflow the bounds:                                                    
  //   v + 4 >= v                                                                                        
  //                                                                                                     
  Expr ge = vc_bvGeExpr(vc, add_v_4, v);

  // Push a new context                                                                                  
  printf("Push\n");
  vc_push(vc);

  // Assert v + 4 = n                                                                                    
  printf("Assert v + 4 = n\n");
  Expr f_add = vc_eqExpr(vc, add_v_4, n);
  vc_assertFormula(vc, f_add);

  // Assert the bounds constraint                                                                        
  printf("Assert v + 4 >= v\n");
  vc_assertFormula(vc, ge);

  // Assert 4 = n                                                                                        
  printf("Assert 4 = n\n");
  Expr f_numeq = vc_eqExpr(vc, ct_4, n);
  vc_assertFormula(vc, f_numeq);

  // Check for satisfiability                                                                            
  printf("Check\n");
  Expr fls = vc_falseExpr(vc);
  int query = vc_query(vc, fls);

  // Pop context                                                                                         
  printf("Pop\n");
  vc_pop(vc);

  printf("query = %d\n", query);
  return 1;
}