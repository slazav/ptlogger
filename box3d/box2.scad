i2m=25.4;

T = 0.7;
dT = 0.02;
// device body (inverted)
L=2*i2m;
W=0.9*i2m;
R=0.1*i2m;
H=8;
dH = 2;

module rbox(L,W,H,R, s1=1, s2=1){
  linear_extrude(height=H, scale=s1){
    // roundings
    for (x=[-1,1], y=[-1,1]){
      translate([x*(L/2-R), y*(W/2-R)])
        circle(r=R, $fn=100);
    }
    // body
    translate([0,0,H/2]){
      square([L-2*R,W], center=true);
      square([L,W-2*R], center=true);
    }
  }
}
module half_box(L,W,H,R){
  hull(){
    rbox(L+2*T,W+2*T,H,R+T);
    translate([0,0,-T])
      rbox(L,W,T,R);
  }
}

module inner_part(){
  rbox(L,W,H+dH+2*T+dT,R); //body
  translate([0,0,H])
    rbox(L+2*T,W+2*T,dH+2*T+dT,R+T);
}



difference(){
  union(){
    half_box(L,W,H,R);
    translate([0,0,H-T]) half_box(L+2*T,W+2*T,dH+3*T,R+T);
  }
  inner_part();
  for (x=[-1,1]){
    translate([0,x*(W/2+T),H+dH+T]) rotate([0,90])
      cylinder(r=T/2, h=L-4*R, center=true, $fn=50);
    translate([x*(L/2+T),0,H+dH+T]) rotate([0,90,90])
      cylinder(r=T/2, h=W-4*R, center=true, $fn=50);
  }
}