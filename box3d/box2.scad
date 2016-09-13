i2m=25.4;

T = 0.8;
dT = 0.02;
// device body (inverted)
L=2*i2m +0.15;
W=0.9*i2m + 0.15;
R=0.1*i2m;
H=8;
dH = 2;

use <inc.scad>

module inner_part(){
  rbox(L,W,H+dH+4*T+dT,R); //body
  translate([0,0,H])
    rbox(L+2*T+0.1,W+2*T+0.1,dH+4*T+dT,R+T);
  // holes
  nh=4;
  for (i=[-nh:nh]){
    translate([L/4,W/4*i/nh,dT]) rotate([180])
      rbox(L*1/3*(1-pow(i/nh,2)/4), T, T+2*dT, T/2);
  }
      
}

difference(){
  union(){
    half_box(L,W,H,R,T);
    translate([0,0,H-T]) half_box(L+2*T,W+2*T,dH+5*T,R+T,T);
  }
  inner_part();
}