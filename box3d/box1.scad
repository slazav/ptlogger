i2m=25.4;

T = 0.8;
dT = 0.02;
// device body (inverted)
L=2*i2m + 0.15;
W=0.9*i2m + 0.15;
R=0.1*i2m;
H=14;
// buttons
Xb = [-6.6,-8.3,-8.3,-8.3]*i2m/10;
Yb = [-3.6,-1.7,0,1.7]*i2m/10;
Rb = [1,1,1,1]*1.25;
Wb = 2.5;
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
Xp = L/2-11;

use <inc.scad>;

module inner_part(){
      rbox(L,W,H+dT,R); //body
      // buttons
      for (i=[0:len(Xb)-1]){
        translate([Xb[i],Yb[i],dT]) rotate([0,180])
          rrbox(Wb,Wb,T+2*dT,Rb[i],3,2.5);
      }
      // screen
      translate([0,0,dT]) rotate([0,180])
        rrbox(Ls,Ws,T+2*dT,Rs,1.2,1.5);
      // usb
      translate([-L/2+dT,0, Zu+Hu/2]) rotate([0,-90])
        rrbox(Hu,Wu,T+2*dT,Ru,1.7,1.7);
      // sd-card
      translate([L/2-dT,0, Zsd+Hsd/2]) rotate([0,90])
        rrbox(Hsd,Wsd,T+2*dT,Rsd,1.7,1.7);
      // power
      translate([-Xp,-W/2+dT, Zp+Hp/2]) rotate([0,90,-90])
        rrbox(Hp,Wp,T+2*dT,Rp,1.1,1.4);
}


difference(){
  union(){
    half_box(L,W,H,R,T);
    translate([0,0,H-4*T]) half_box(L+2*T,W+2*T,T,R+T,T);
  }
  inner_part();
}

//        %rbox(Hp,Wp,T+2*dT,Rp,1.4,1.4);
//      rrbox(Hp,Wp,T+2*dT,Rp,1.4,1.4);
