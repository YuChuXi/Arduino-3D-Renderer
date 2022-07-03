#include <U8glib.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0); // 实例化屏幕

float FOC = 100;       // 焦距
float tine = millis(); // 计时器

struct DD_pip { // 一个2维点
  float x, y;
};

struct TD_pip { // 一个三维点
  float x, y, z;
};

struct CAM {          // 一个摄像机
  struct TD_pip pip { // 摄像机位置
    0, -20, 0
  };
  float a = 0; // 摄像机方向
  float b = 0; //
};

struct DD_pip TD_toDD(struct TD_pip in) { //相对坐标换算
  DD_pip out {                            // 将要返回的点
      FOC * in.x / (FOC + in.y),
      -1 * FOC * in.z / (FOC + in.y),
  };
  return out;
}

// 旋转
struct TD_pip REV_x(struct TD_pip in, float a) { // X轴
  float s = sin(a);
  float c = cos(a);
  TD_pip out { // 将要返回的点
      in.x,
      in.y * c - in.z * s,
      in.y * s + in.z * c,
  };
  return out;
}
struct TD_pip REV_y(struct TD_pip in, float a) {  //Y轴
  float s = sin(a);
  float c = cos(a);
  TD_pip out { // 将要返回的点
      in.z * c - in.x * s,
      in.y,
      in.z * s + in.x * c,
  };
  return out;
}
struct TD_pip REV_z(struct TD_pip in, float a) { // Z轴
  float s = sin(a);
  float c = cos(a);
  TD_pip out { // 将要返回的点
      in.x * c - in.y * s,
      in.x * s + in.y * c,
      in.z,
  };
  return out;
}

struct TD_pip W_to_C(struct TD_pip w, struct CAM c) { // 绝对坐标换相对坐标
  TD_pip out { // 将要返回的点
      w.x - c.pip.x,
      w.y - c.pip.y,
      w.z - c.pip.z,
  };
  out = REV_z(REV_x(out, -1 * c.a), -1 * c.b);
  return out;
}

void PutLine(DD_pip a, DD_pip b) { // 画线
  u8g.drawLine(a.x + 64, a.y + 32, b.x + 64, b.y + 32);
}

CAM user; // 定义用户摄像机

// 立方体的点
TD_pip p1{20, 20, 20};
TD_pip p2{10, 10, -10};
TD_pip p3{10, -10, 10};
TD_pip p4{10, -10, -10};
TD_pip p5{-10, 10, 10};
TD_pip p6{-10, 10, -10};
TD_pip p7{-10, -10, 10};
TD_pip p8{-10, -10, -10};

void setup() {
  Serial.begin(9600);
}

void loop() {
  char str[5];                 // fps字符串缓存
  u8g.setFont(u8g_font_6x10r); // 设置字体
  u8g.firstPage();             // 开始循环
  do {
    // 转座标
    DD_pip n1 = TD_toDD(W_to_C(p1, user));
    DD_pip n2 = TD_toDD(W_to_C(p2, user));
    DD_pip n3 = TD_toDD(W_to_C(p3, user));
    DD_pip n4 = TD_toDD(W_to_C(p4, user));
    DD_pip n5 = TD_toDD(W_to_C(p5, user));
    DD_pip n6 = TD_toDD(W_to_C(p6, user));
    DD_pip n7 = TD_toDD(W_to_C(p7, user));
    DD_pip n8 = TD_toDD(W_to_C(p8, user));

    // 画线
    PutLine(n1, n2);
    PutLine(n2, n4);
    PutLine(n3, n4);
    PutLine(n3, n1);
    PutLine(n1, n5);
    PutLine(n2, n6);
    PutLine(n3, n7);
    PutLine(n4, n8);
    PutLine(n5, n6);
    PutLine(n6, n8);
    PutLine(n7, n8);
    PutLine(n7, n5);

    // user.b=user.b+0.01; // 转摄像机 

    //旋转
    ///*
    p1 = REV_x(p1, 0.01);
    p2 = REV_x(p2, 0.01);
    p3 = REV_x(p3, 0.01);
    p4 = REV_x(p4, 0.01);
    p5 = REV_x(p5, 0.01);
    p6 = REV_x(p6, 0.01);
    p7 = REV_x(p7, 0.01);
    p8 = REV_x(p8, 0.01);
    //*/
    ///*
    p1 = REV_z(p1, 0.001);
    p2 = REV_z(p2, 0.001);
    p3 = REV_z(p3, 0.001);
    p4 = REV_z(p4, 0.001);
    p5 = REV_z(p5, 0.001);
    p6 = REV_z(p6, 0.001);
    p7 = REV_z(p7, 0.001);
    p8 = REV_z(p8, 0.001);
    //*/


    // u8g.draw
    //  fps
    //  Serial.print("x"); Serial.print(n1.x - 64); Serial.print("y"); Serial.print(n1.y - 32); Serial.print("x"); Serial.print( n2.x - 64); Serial.print("y"); Serial.println( n2.y - 32);
    
    // 打印fps
    itoa(1000 / ((float)millis() - tine), str, 10);
    u8g.drawStr(0, 10, str);
    tine = millis();

  } while (u8g.nextPage());
}
