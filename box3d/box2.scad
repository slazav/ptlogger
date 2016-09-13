i2m=25.4;

T = 0.7;
dT = 0.02;
// device body (inverted)
L=2*i2m;
W=0.9*i2m;
R=0.1*i2m;
H=8;
dH = 2;

use <inc.scad>

module inner_part(){
  rbox(L,W,H+dH+2*T+dT,R); //body
  translate([0,0,H])
    rbox(L+2*T,W+2*T,dH+2*T+dT,R+T);
  // holes
  nh=4;
  for (i=[-nh:nh]){
    translate([L/4,W/4*i/nh,dT]) rotate([180])
      rrbox(L*1/3*(1-pow(i/nh,2)/4), T, T+2*dT, T/2, 1.2,1.2);
  }
      
}

difference(){
  union(){
    half_box(L,W,H,R,T);
    translate([0,0,H-T]) half_box(L+2*T,W+2*T,dH+3*T,R+T,T);
  }
  inner_part();
  for (x=[-1,1]){
    translate([0,x*(W/2+T),H+dH+T]) rotate([0,90])
      cylinder(r=T/2, h=L-4*R, center=true, $fn=50);
    translate([x*(L/2+T),0,H+dH+T]) rotate([0,90,90])
      cylinder(r=T/2, h=W-4*R, center=true, $fn=50);
  }
}