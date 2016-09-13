i2m=25.4;

T = 0.7;
dT = 0.02;
// device body (inverted)
L=2*i2m;
W=0.9*i2m;
R=0.1*i2m;
H=14;
// inner pins
Rpin=0.09/2*i2m;
Hpin=4;
// buttons
Xb = [-6.6,-8.3,-8.3,-8.3]*i2m/10;
Yb = [-3.6,-1.7,0,1.7]*i2m/10;
Rb = [1,1,0.5,1];
Wb = 2;
// screen
Ls = 0.91*i2m;
Ws = 0.25*i2m;
Xs = -0.4*i2m;
Ys = -0.16*i2m;
Rs = 0.5;
// USB
Zu = 7.75;
Hu = 2;
Wu = 7;
Ru = 0.5;
//sd-card
Zsd = 7.75;
Hsd = 2;
Wsd = 11.5;
Rsd = 0.5;
//power
Zp = 5;
Hp = 5.2;
Wp = 6;
Rp = 0.5;
Xp = L/2-14;

use <inc.scad>;

module inner_part(){
  difference(){
    union(){
      rbox(L,W,H+dT,R); //body
      // buttons
      for (i=[0:len(Xb)-1]){
        translate([Xb[i],Yb[i],dT]) rotate([0,180])
          rrbox(Wb,Wb,T+2*dT,Rb[i],3,2.5);
      }
      // screen
      translate([0,0,dT]) rotate([0,180])
        rrbox(Ls,Ws,T+2*dT,Rs,1.5,1.5);
      // usb
      translate([-L/2+dT,0, Zu+Hu/2]) rotate([0,-90])
        rrbox(Hu,Wu,T+2*dT,Ru,2,2);
      // sd-card
      translate([L/2-dT,0, Zsd+Hsd/2]) rotate([0,90])
        rrbox(Hsd,Wsd,T+2*dT,Rsd,2,2);
      // power
      translate([-Xp,-W/2+dT, Zp+Hp/2]) rotate([0,90,-90])
        rrbox(Hp,Wp,T+2*dT,Rp,1.4,1.4);
    }
    for (x=[-1,1], y=[-1,1]){
      translate([x*(L/2-R), y*(W/2-R),-dT])
        cylinder(r1=Rpin, r2=Rpin/2, h=Hpin, $fn=50);
    }
  }
}


difference(){
  union(){
    half_box(L,W,H,R,T);
    translate([0,0,H-3*T]) half_box(L+2*T,W+2*T,T,R+T,T);
    for (x=[-1,1]){
      translate([0,x*(W/2+T),H-T]) rotate([0,90])
        cylinder(r=T/2, h=L-4*R, center=true, $fn=50);
      translate([x*(L/2+T),0,H-T]) rotate([0,90,90])
        cylinder(r=T/2, h=W-4*R, center=true, $fn=50);
    }
  }
  inner_part();
}

//        %rbox(Hp,Wp,T+2*dT,Rp,1.4,1.4);
//      rrbox(Hp,Wp,T+2*dT,Rp,1.4,1.4);
