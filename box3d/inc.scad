// flat sqare with rounded corners
module rsquare(L,W,R){
  // roundings
  for (x=[-1,1], y=[-1,1]){
    translate([x*(L/2-R), y*(W/2-R)])
      circle(r=R, $fn=100);
  }
  // body
  translate([0,0]){
    square([L-2*R,W], center=true);
    square([L,W-2*R], center=true);
  }
}

// same, but extruded into a box
module rbox(L,W,H,R, s=1){
  linear_extrude(height=H, scale=s) rsquare(L,W,R);
}

// rounded box with non-linear extruding
rri=5;
module rrbox(L,W,H,R, SL,SW){
  ddSL = pow(SL,2/rri/(rri-1));
  ddSW = pow(SW,2/rri/(rri-1));
  dH=H/rri;

  for (i=[0:rri-1]){
    dSL = pow(ddSL,i);
    dSW = pow(ddSW,i);
    L1=(i>0)? L*pow(ddSL, (i-1)*i/2):L;
    W1=(i>0)? W*pow(ddSW, (i-1)*i/2):W;
    R1=(i>0)? R*pow(ddSW, (i-1)*i/2):R;

    translate([0,0,i*dH])
    if (i<rri-1){ // avoid 0-walls
      g = 1.2; // overlap
      S = [1+g*(dSL-1),1+g*(dSW-1)];
      linear_extrude(height=dH*g, scale=S)
      rsquare(L1,W1,R1);
    }
    else {
      linear_extrude(height=dH, scale=[dSL,dSW])
      rsquare(L1,W1,R1);
    }
  }
}


// same, but with rounded bottom
module half_box(L,W,H,R,T){
  hull(){
    rbox(L+2*T,W+2*T,H,R+T);
    translate([0,0,-T])
      rbox(L,W,T,R);
  }
}
