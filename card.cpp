#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <cmath>
#include <vector>
#include <string>

int winWidth = 1000;
int winHeight = 750;
float starRotate = 0;
float cloudMove = 0;
float angle = 0;

struct Box {
    float x, y, w, h;
    std::string text; 
    bool clicked = false;  // 方框是否已被点击
    bool isActive = false;  // 方框是否已被选中
};

std::vector<Box> boxes = {
    {70, 350, 50, 50, "X"},
    {180, 300, 50, 50, "J"},
    {330, 200, 50, 50, "T"},
    {470, 300, 50, 50, "L"},
    {570, 350, 50, 50, "U"}
};

// 画圆
void drawCircle(float radius) {
    int sides = 32;
    glBegin(GL_POLYGON);
    for (int i = 0; i < sides; i++) {
        float angles = 2.0 * 3.14159265358979323846 * float(i) / float(sides);
        float x = radius * std::cos(angles);
        float y = radius * std::sin(angles);
        glVertex2f(x, y);
    }
    glEnd();
}

// 设置字体样式
void selectFont(int size, int charset, const char* face)
{
    HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
        charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face); // 创建一个字体对象
    HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont); // 将其关联到设备上下文用于绘图并返回原先选择的字体
    DeleteObject(hOldFont);
}

// 绘制文字
void drawString(const char* str)
{
    int len, i;
    wchar_t* wstring;
    HDC hDC = wglGetCurrentDC();
    GLuint list = glGenLists(1); // 创建显示列表存放绘图命令

    len = 0;
    for (i = 0; str[i] != '\0'; ++i)
    {
        if (IsDBCSLeadByte(str[i]))
            ++i;  // 判断当前字符是否为双字节字符的首字节
        ++len;
    }

    wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t)); // 分配内存
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len); // 多字节字符串转换为宽字符字符串

    for (i = 0; i < len; ++i)
    {
        wglUseFontBitmapsW(hDC, wstring[i], 1, list);
        glCallList(list); // 绘制每个字符
    }

    free(wstring);
    glDeleteLists(list, 1);
}

// 绘制树木
void tree() {
    glPushMatrix();

    glBegin(GL_TRIANGLES);
    // 树干
    glColor3ub(139, 69, 19);
    glVertex2f(-0.1, 0);
    glVertex2f(0.1, 0);
    glVertex2f(0.1, 0.5);

    glVertex2f(-0.1, 0);
    glVertex2f(0.1, 0.5);
    glVertex2f(-0.1, 0.5);

    // 树叶
    glColor3ub(50, 205, 50);
    glVertex2f(-0.4, 0.4);
    glVertex2f(0.4, 0.4);
    glVertex2f(0.0, 0.7);

    glVertex2f(-0.38, 0.6);
    glVertex2f(0.38, 0.6);
    glVertex2f(0.0, 1.0);

    glVertex2f(-0.35, 0.9);
    glVertex2f(0.35, 0.9);
    glVertex2f(0.0, 1.3);
    glEnd();

    glPopMatrix();
}

// 绘制云朵
void cloud() {
    glPushMatrix();

    glColor3ub(187, 255, 255);
    glTranslated(-0.3, 0, 0);
    drawCircle(0.08);

    glTranslated(0.13, 0, 0);
    drawCircle(0.1);

    glTranslated(0.1, -0.04, 0);
    drawCircle(0.09);

    glTranslated(0.1, 0.06, 0);
    drawCircle(0.09);

    glTranslated(0.09, -0.03, 0);
    drawCircle(0.07);

    glPopMatrix();
}

// 绘制鲜花
void flower(float r, float g, float b) {
    glPushMatrix();

    // 枝干
    glColor3ub(0, 139, 0);
    glBegin(GL_QUADS);
    glVertex2f(-0.01, 0);
    glVertex2f(0.01, 0);
    glVertex2f(0.01, -0.35);
    glVertex2f(-0.01, -0.35);
    glEnd();

    // 树叶
    glPushMatrix();
    glTranslatef(-0.05, -0.28, 0);
    glRotated(-30, 0, 0, 1);
    glScaled(2.2, 1.0, 0);
    drawCircle(0.03);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.05, -0.28, 0);
    glRotated(30, 0, 0, 1);
    glScaled(2.2, 1.0, 0);
    drawCircle(0.03);
    glPopMatrix();

    // 花瓣和花心
    glColor3f(r, g, b);
    glPushMatrix();
    for (int i = 0; i < 10; ++i) {
        glPushMatrix();
        glRotated(i * 36, 0, 0, 1);
        glTranslated(0.12, 0, 0);
        glScaled(2.2, 1.1, 0);
        drawCircle(0.03);
        glPopMatrix();
    }

    glColor3ub(255, 130, 71);
    drawCircle(0.07);
    glPopMatrix();

    glPopMatrix();
}

// 房子
void house() {
    glPushMatrix();

    // 中间
    glColor3ub(0, 191, 255);
    glBegin(GL_POLYGON);
    glVertex2d(-0.4, 0.1);
    glVertex2d(0.45, 0.1);
    glVertex2d(0.45, 1.15);
    glVertex2d(-0.4, 1.15);
    glEnd();

    // 底部
    glColor3ub(112, 128, 144);
    glBegin(GL_POLYGON);
    glVertex2d(-0.5, 0);
    glVertex2d(0.5, 0);
    glVertex2d(0.5, 0.4);
    glVertex2d(0.3, 0.4);
    glVertex2d(-0.5, 0.15);
    glEnd();

    // 顶部
    glBegin(GL_POLYGON);
    glVertex2d(-0.5, 0.54);
    glVertex2d(-0.1, 0.54);
    glVertex2d(-0.1, 1.0);
    glVertex2d(-0.2, 1.0);
    glVertex2d(-0.25, 1.2);
    glVertex2d(-0.5, 1.2);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(-0.1, 0.54);
    glVertex2d(0.18, 0.74);
    glVertex2d(0.18, 1.2);
    glVertex2d(-0.1, 1.2);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(0.18, 0.74);
    glVertex2d(0.35, 0.74);
    glVertex2d(0.35, 1.2);
    glVertex2d(0.18, 1.2);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(0.35, 0.74);
    glVertex2d(0.42, 0.54);
    glVertex2d(0.5, 0.54);
    glVertex2d(0.5, 1.2);
    glVertex2d(0.35, 1.2);
    glEnd();

    // 红边
    glColor3ub(205, 85, 85);
    glLineWidth(4);
    glBegin(GL_LINE_STRIP);
    glVertex2d(-0.5, 0.54);
    glVertex2d(-0.1, 0.54);
    glVertex2d(0.18, 0.74);
    glVertex2d(0.35, 0.74);
    glVertex2d(0.42, 0.54);
    glVertex2d(0.5, 0.54);
    glVertex2d(0.5, 1.2);
    glVertex2d(-0.1, 1.2);
    glVertex2d(-0.1, 1.0);
    glVertex2d(-0.2, 1.0);
    glVertex2d(-0.25, 1.2);
    glVertex2d(-0.5, 1.2);
    glVertex2d(-0.5, 0.54);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2d(-0.5, 0);
    glVertex2d(0.5, 0);
    glVertex2d(0.5, 0.4);
    glVertex2d(0.3, 0.4);
    glVertex2d(-0.5, 0.15);
    glVertex2d(-0.5, 0);
    glEnd();

    glLineWidth(1);

    glPopMatrix();
}

// 五角星
void drawFiveStar() {
    glPushMatrix();
    glPushMatrix();
    float r = 0.07;
    float x[6], y[6];
    for (int i = 0; i < 5; ++i) {
        x[i] = r * cos(i * 1.f / 5 * 2 * 3.1415926f);
        y[i] = r * sin(i * 1.f / 5 * 2 * 3.1415926f);
    }

    glRotated(17.5, 0, 0, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x[0], y[0]);
    glVertex2f(x[2], y[2]);
    glVertex2f(x[4], y[4]);
    glVertex2f(x[1], y[1]);
    glVertex2f(x[3], y[3]);
    glEnd();

    glLineWidth(1);

    glPopMatrix();
    glPopMatrix();
}


// 纸飞机
void plane() {
    glPushMatrix();

    glColor3ub(224, 255, 255);
    glBegin(GL_TRIANGLES);
    glVertex2d(0, 0);
    glVertex2d(0.1, 0.25);
    glVertex2d(0.25, -0.3);

    glVertex2d(-0.2, 0.02);
    glVertex2d(0, 0);
    glVertex2d(0.25, -0.3);

    glVertex2d(-0, -0.0);
    glVertex2d(-0.05, -0.15);
    glVertex2d(0.25, -0.3);
    glEnd();

    // 线条
    glLineWidth(3);
    glColor3ub(139, 101, 8);
    glBegin(GL_LINE_LOOP);
    glVertex2d(0, 0);
    glVertex2d(0.1, 0.25);
    glVertex2d(0.25, -0.3);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2d(-0.2, 0.02);
    glVertex2d(0, 0);
    glVertex2d(0.25, -0.3);
    glEnd();

    glLineWidth(1);

    glPopMatrix();
}


// 控制点
std::vector<std::vector<float>> myPoints = {
     {-0.9, 0.5},
     {-0.6, 0.1},
     {-0.3, 0.2},
     {-0.25, 0.224},
     {0.0, -0.2},
     {0.3, -0.9},
     {0.7, -0.8},
     {0.9, -0.77},
};
std::vector<std::vector<float>> bezierPoint; // 存储曲线上的点
int curCnt = 0; // 当前点

// 绘制学士帽
void caps() {
    glPushMatrix();

    //上部
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(-0.5, 0.4);
    glVertex2f(0.0, 0.6);
    glVertex2f(0.5, 0.4);
    glVertex2f(0.0, 0.3);
    glEnd();

    //下部
    glBegin(GL_POLYGON);
    glVertex2f(-0.18, 0.15);    
    glVertex2f(0.0, 0.3);
    glVertex2f(0.18, 0.15);
    glVertex2f(0.15, 0.33);
    glVertex2f(-0.15, 0.33);
    glEnd();

    //穗
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(2.5);
    glBegin(GL_LINES);
    glVertex2f(-0.3, 0.35);
    glVertex2f(-0.3, 0.1);
    glEnd();

    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.3, 0.1);
    glVertex2f(-0.35, -0.05);
    glVertex2f(-0.25, -0.05);
    glEnd();

    // 白边
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(4);
    glBegin(GL_LINE_STRIP);
    glVertex2d(-0.5, 0.4);
    glVertex2d(0.0, 0.6);
    glVertex2d(0.5, 0.4);
    glVertex2d(0.0, 0.3);
    glVertex2d(-0.5, 0.4);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(4);
    glBegin(GL_LINE_STRIP);
    glVertex2d(0.15, 0.33); 
    glVertex2d(0.0, 0.3);
    glVertex2d(-0.15, 0.33);
    glVertex2d(-0.18, 0.15);    
    glVertex2d(0.18, 0.15);
    glVertex2d(0.15, 0.33);
    glEnd();

    glLineWidth(1);

    glPopMatrix();
}

// 绘制贺卡
void card() {
    glPushMatrix();
    glTranslated(winWidth * 0.5, winHeight * 0.5, 0); // 坐标原点从左下角到中心
    glScaled(winWidth * 0.5, winHeight * 0.5, 1);

    // 蓝天
    glBegin(GL_QUADS);
    glColor3f(0.6, 0.8, 0.9);
    glVertex2f(-1.f, -1.0);
    glColor3f(0.6, 0.8, 0.9);
    glVertex2f(1.f, -1.0);
    glColor3f(0.0, 0.2, 0.68);
    glVertex2f(1.f, 1.f);
    glVertex2f(-1.0, 1.f);
    glEnd();

    // 草地
    {
        glColor3ub(60, 179, 113);
        glBegin(GL_QUADS);
        for (int i = 0; i < 200; ++i) {
            float x1 = i * 1.f / 100 * 2 - 1;
            float x2 = (i + 1) * 1.f / 100 * 2 - 1;
            float y1 = 0.06 * (x1 - 0.9) * (x1 - 0.9) - 0.66;
            float y2 = 0.06 * (x2 - 0.9) * (x2 - 0.9) - 0.66;
            glVertex2f(x1, -1.f);
            glVertex2f(x2, -1.f);
            glVertex2f(x2, y1);
            glVertex2f(x1, y2);
        }
        glEnd();
    }

    // 树木
    glPushMatrix();
    glTranslatef(-0.8, -0.7, 0);
    glScaled(0.3, 0.3, 0.3);
    tree();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.15, -0.75, 0);
    glScaled(0.3, 0.3, 0.3);
    tree();
    glPopMatrix();

    // 房子
    glPushMatrix();
    glTranslated(0.66, -0.7, 0);
    glScaled(0.6, 0.6, 0.6);
    house();
    glPopMatrix();

    // 文字
    {
        selectFont(80, ANSI_CHARSET, "Comic Sans MS");
        glColor3ub(250, 128, 114);
        glRasterPos2f(-0.7f, 0.23f);
        drawString(" Congratulations!");

        selectFont(50, ANSI_CHARSET, "Comic Sans MS");
        glColor3ub(220, 200, 110);
        glRasterPos2f(-0.6f, -0.68f);
        drawString("Welcome to 2024");

        selectFont(50, ANSI_CHARSET, "Comic Sans MS");
        glColor3ub(220, 200, 110);
        glRasterPos2f(-0.65f, -0.93f);
        drawString("Graduation Ceremony");

        selectFont(40, ANSI_CHARSET, "Comic Sans MS");
        glColor3ub(107, 255, 205);
        glRasterPos2f(0.36f, 0.2f);
        drawString("Time: 2024.7.15 14:00");
        glRasterPos2f(0.46f, 0.1f);
        drawString("Venue: CBG13W");

        selectFont(25, ANSI_CHARSET, "Comic Sans MS");
        glColor3ub(0, 0, 128);
        glRasterPos2f(0.75f, -0.1f);
        drawString(" XJTLU");

        selectFont(37, ANSI_CHARSET, "Comic Sans MS");
        glColor3ub(205, 205, 193);
        glRasterPos2f(0.5f, -0.84f);
        drawString("Your Journey:");
        glRasterPos2f(0.52, -0.95f);
        drawString("2020 - 2024");
    }

    // 贝塞尔曲线
    {
        glColor3ub(0, 206, 209);
        glLineWidth(1);
        glBegin(GL_LINES);
        for (int i = 0; i < bezierPoint.size() - 1; ++i) {
            glVertex2f(bezierPoint[i][0], bezierPoint[i][1]); // 第i个点的x y坐标
            glVertex2f(bezierPoint[i + 1][0], bezierPoint[i + 1][1]);
        }
        glEnd();
    }

    // 飞机
    glPushMatrix();
    glTranslatef(bezierPoint[curCnt][0], bezierPoint[curCnt][1], 0.f);
    glScaled(0.5, 0.5, 0.5);
    plane();
    glPopMatrix();

    glPopMatrix();

    glPushMatrix();
    glTranslated(winWidth * 0.5, winHeight * 0.5, 0);
    glScaled(winHeight * 0.5, winHeight * 0.5, 1);

    // 三朵云
    glPushMatrix();
    glTranslated(-0.7 + cloudMove, 0.8, 0);
    cloud();
    glTranslated(0.9 + cloudMove, -0.2, 0);
    cloud();
    glTranslated(0.8 + cloudMove, 0.3, 0);
    cloud();
    glPopMatrix();

    // 左花
    glPushMatrix();
    glTranslated(0.6, -0.82, 0);
    glScaled(0.35, 0.35, 0.35);
    flower(0.72, 0.36, 0.73);
    glPopMatrix();

    // 右花
    glPushMatrix();
    glTranslated(1.2, -0.82, 0);
    glScaled(0.35, 0.35, 0.35);
    flower(0.42, 0.26, 0.873);
    glPopMatrix();

    // 两个五角星
    glPushMatrix();
    glTranslated(-1.02, 0.29, 0);
    glPushMatrix();
    glRotated(starRotate, 0, 1, 0);
    glColor3f(1, 1, 0);
    drawFiveStar();
    glPopMatrix();

    glTranslated(1.25, 0.0, 0);
    glPushMatrix();
    glRotated(starRotate, 0, 1, 0);
    drawFiveStar();
    glPopMatrix();
    glPopMatrix();

    // 绘制帽子
    glTranslated(-0.4, -0.5, 0);
    glPushMatrix();
    glRotated(angle, 0, 0, 1);
    caps();
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

// 鼠标交互
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        angle -= 72.0;
        if (angle < 0.0) {
            angle += 360.0;
        }
        glutPostRedisplay();

        y = winHeight - y;  // 坐标原点在左下角
        for (Box& box : boxes) { 
            if (x >= box.x && x <= box.x + box.w && y >= box.y - box.h && y <= box.y) {
                box.isActive = true;
                box.clicked = true;
            }
            else {
                box.isActive = false;
            }
        }
    }
}

// 键盘交互
void keyboard(unsigned char key, int x, int y) {
    float moveDistance = 10.0f; 

    switch (key) {
    case 'a':
    case 'A':
        for (Box& box : boxes) {
            if (box.isActive) {
                box.x -= moveDistance;
                break;
            }
        }
        break;
    case 'd':
    case 'D':
        for (Box& box : boxes) {
            if (box.isActive) {
                box.x += moveDistance;
                break;
            }
        }
        break;
    case 'w':
    case 'W':
        for (Box& box : boxes) {
            if (box.isActive) {
                box.y += moveDistance;
                break;
            }
        }
        break;
    case 's':
    case 'S':
        for (Box& box : boxes) {
            if (box.isActive) {
                box.y -= moveDistance;
                break;
            }
        }
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION); // 先正交投影控制视角
    glLoadIdentity();
    gluOrtho2D(0, winWidth, 0, winHeight); 
    glMatrixMode(GL_MODELVIEW);  // 然后绘制物体
    glLoadIdentity();

    card();

    for (const Box& box : boxes) {
        glColor3f(1.0f, 0.8f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(box.x, box.y);
        glVertex2f(box.x + box.w, box.y);
        glVertex2f(box.x + box.w, box.y - box.h);
        glVertex2f(box.x, box.y - box.h);
        glEnd();

        if (box.clicked) {
            glColor3f(0.0f, 0.0f, 0.8f);
        }
        else {
            glColor3f(1.0f, 0.8f, 0.0f);
        }
        glRasterPos2f(box.x + box.w / 3, box.y - box.h / 1.5);
        for (char c : box.text) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    }

    glutSwapBuffers();
}

void init() {
    glClearColor(255 / 255.f, 245 / 255.f, 238 / 255.f, 1.0);

    // 计算贝塞尔曲线的点的坐标
    int num = myPoints.size() - 1; // 曲线度数
    for (float t = 0; t < 1.0; t += 1.0 / (300)) // 300段
    {
        std::vector<std::vector<float>> temp = myPoints; // 存储控制点副本

        for (int i = 0; i < num; ++i) {
            for (int j = num; j > i; j--) {
                temp[j][0] = (1 - t) * temp[j - 1][0] + t * temp[j][0];
                temp[j][1] = (1 - t) * temp[j - 1][1] + t * temp[j][1];
            }
        }
        bezierPoint.push_back(temp[num]);
    }
}

// 定时器处理函数
void onTimer(int value) 
{
    curCnt = (curCnt + 1) % bezierPoint.size();
    starRotate += 3;
    static int tmp = 0;
    tmp += 1;
    cloudMove = 0.1 * sin(tmp * 0.03);
    glutTimerFunc(20, onTimer, 1);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
    glutInitWindowSize(winWidth, winHeight); 
    glutCreateWindow("2024 XJTLU Graduation Ceremony");

    init(); 

    glutTimerFunc(20, onTimer, 1);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}