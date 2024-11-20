#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <GL/glut.h>			
#include <gl/glaux.h>		
#pragma comment (lib, "glaux.lib")
#pragma comment (lib, "legacy_stdio_definitions.lib")

// 定义全局变量 
int last_x = 0;
int last_y = 0;
float rotate_x = 0.0f;
float rotate_y = 0.0f;
float rot = 0.0;
float xtrans = 0.0;
float ytrans = 0.0;
float ztrans = -10.0;
float scale = 1.0;
unsigned int textureID;

void loadGLTexture(const char* filename) {

    AUX_RGBImageRec* TextureImage;
    TextureImage = auxDIBImageLoad(filename);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
}

void renderText(const char* text, float x, float y, float z) {
    glPushMatrix();
    glRasterPos3f(x, y, z);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    glPopMatrix();
}

// 初始化函数
void init() {
    // 设置背景颜色
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0f);
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // 启用深度测试、背面剔除、光照等设置
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_LIGHTING);
    // 设置光照颜色、位置等参数
    GLfloat LightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };			//环境光参数
    GLfloat LightDiffuse[] = { .3f, 0.5f, 0.2f, 1.0f };			//漫反射光参数
    GLfloat LightPosition[] = { -10.0f, 0.0f, 2.0f, 0.0f };			//光源位置
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// 设置环境光源
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// 设置漫反射光源
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);	// 设置光源位置
    glEnable(GL_LIGHT1);								// 启动光源1
    loadGLTexture("Dog.bmp");
}

// 显示回调函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // 转换视角
    glTranslatef(xtrans, ytrans, ztrans);
    glRotatef(rotate_x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotate_y, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glPushMatrix();
    glutSolidTorus(0.5,2,32,32);
    renderText("This could change", 0.0f, 0.0f, 0.0f);
    glPopMatrix();
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);


    glutSwapBuffers();
}
// 鼠标移动回调函数
void motion(int x, int y) {
    // 计算鼠标移动距离
    int delta_x = x - last_x;
    int delta_y = y - last_y;
    // 更新上一次的位置信息
    last_x = x;
    last_y = y;
    // 根据鼠标移动距离更新旋转角度
    rotate_x += (float)delta_y;
    rotate_y += (float)delta_x;
    glutPostRedisplay();
}
// 鼠标按下回调函数
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // 记录上一次的位置信息
        last_x = x;
        last_y = y;
    }
}

void onKeyboard(unsigned char key, int x, int y) {

    if (key == 'W' || key == 'w') {

        ytrans += 0.03;
    }
    if (key == 'S' || key == 's') {

        ytrans -= 0.03;
    }
    if (key == 'a' || key == 'A') {

        xtrans -= 0.03;
    }
    if (key == 'D' || key == 'd') {

        xtrans += 0.03;
    }

    if (key == 'Q' || key == 'q') {

        ztrans += 0.03;
    }

    if (key == 'E' || key == 'e') {
        ztrans -= 0.03;

    }
    if (key == 'L' || key == 'l')
    {
        scale += 0.03;
    }
    if (key == 'K' || key == 'k')
    {
        scale -= 0.03;
    }
    glutPostRedisplay();
}
// 窗口大小回调函数
void reshape(int width, int height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("OpenGL");
    glutDisplayFunc(display);
    glutKeyboardFunc(onKeyboard);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    init();
    glutMainLoop();
    return 0;
}