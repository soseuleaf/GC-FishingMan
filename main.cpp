#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX 100
#define c(i) i / 255.0f

struct MotionInfo {
    int start;
    int end;
    int tick;
    int maxTick;
    float val;
    float* linkedVal;
    MotionInfo* nextInfo;
    bool canMotion = false;
};

struct Queue {
    MotionInfo data[MAX];
    int front = -1;
    int rear = -1;
};

enum Fish {
    Sphere,
    Cube,
    Torus,
    Cone,
    Icosahedron,
    Teapot,
    RealFish,
    FishMax
};

void queueInit(Queue* q) {
    q->front = -1;
    q->rear = -1;
}

int isEmpty(Queue q) {
    if (q.front == q.rear)//front와 rear가 같으면 큐는 비어있는 상태 
        return true;
    else return false;
}

int isFull(Queue q) {
    int tmp = (q.rear + 1) % MAX; //원형 큐에서 rear+1을 MAX로 나눈 나머지값이
    if (tmp == q.front)//front와 같으면 큐는 가득차 있는 상태 
        return true;
    else
        return false;
}

void enQueue(Queue* q, MotionInfo value) {
    if (isFull(*q))
        printf("Queue is Full.\n");
    else {
        q->rear = (q->rear + 1) % MAX;
        q->data[q->rear] = value;
    }
}
MotionInfo deQueue(Queue* q) {
    if (isEmpty(*q))
        printf("Queue is Empty.\n");
    else {
        q->front = (q->front + 1) % MAX;
        return q->data[q->front];
    }
}

int MX = 300;
int MY = 300;
float moveX = 0;
float moveY = 0;
float testrotate = 0;
float rotate = 0;

float catchR, catchG, catchB;
Fish catchFish;

bool motioning = false;
bool fishing = false;
bool hit = false;
bool catching = false;

float zziRoatate = -80;
float armLXRotate = 0;
float armLYRotate = 0;
float armRXRotate = 100;
float armRYRotate = 0;
float bodyRotate = 0;
float legLXRotate = 0;
float legLYRotate = 0;
float legRXRotate = 0;
float legRYRotate = 0;
float zzi_vib = 0;
float zziSize = 0.5;
float zziSaveTime = 0;

// 미끼 부들
MotionInfo zzi_vib_1, zzi_vib_2, zzi_vib_3, zzi_vib_4;

// idle 모션
MotionInfo arm_l_move_1, arm_l_move_2, arm_l_move_3;
MotionInfo arm_r_move_1, arm_r_move_2, arm_r_move_3;
MotionInfo leg_l_move_1, leg_l_move_2, leg_l_move_3;
MotionInfo leg_r_move_1, leg_r_move_2, leg_r_move_3;

// 던지기 모션
MotionInfo arm_1, arm_2;
MotionInfo zzi_1, zzi_2;
MotionInfo body_1, body_2;
MotionInfo leg_l_1, leg_l_2;
MotionInfo leg_r_1, leg_r_2;

// 낚기 모션
MotionInfo zzi_catch_1, zzi_catch_2, zzi_catch_2_1, zzi_catch_3;

MotionInfo body_catch_1, body_catch_2, body_catch_2_1, body_catch_3;

MotionInfo arm_l_x_catch_1, arm_l_x_catch_2, arm_l_x_catch_2_1, arm_l_x_catch_3;
MotionInfo arm_l_y_catch_1, arm_l_y_catch_2, arm_l_y_catch_2_1, arm_l_y_catch_3;
MotionInfo arm_r_x_catch_1, arm_r_x_catch_2, arm_r_x_catch_2_1, arm_r_x_catch_3;
MotionInfo arm_r_y_catch_1, arm_r_y_catch_2, arm_r_y_catch_2_1, arm_r_y_catch_3;

MotionInfo leg_l_x_catch_1, leg_l_x_catch_2, leg_l_x_catch_2_1, leg_l_x_catch_3;
MotionInfo leg_l_y_catch_1, leg_l_y_catch_2, leg_l_y_catch_2_1, leg_l_y_catch_3;
MotionInfo leg_r_x_catch_1, leg_r_x_catch_2, leg_r_x_catch_2_1, leg_r_x_catch_3;
MotionInfo leg_r_y_catch_1, leg_r_y_catch_2, leg_r_y_catch_2_1, leg_r_y_catch_3;

// 해달
MotionInfo sunmoon_1;
float sunmoonRotate = 0;

Queue rotateQueue;
Queue rotateQueueTmp;
GLUquadricObj* p;

void dataInit() {
    srand(time(NULL));

    arm_1 = { 100, 195, 0, 50};
    arm_1.linkedVal = &armRXRotate;
    arm_1.nextInfo = &arm_2;

    arm_2 = { 195,  45, 0, 20 };
    arm_2.linkedVal = &armRXRotate;
    arm_2.nextInfo = NULL;
    arm_2.canMotion = true;

    zzi_1 = { 0, 100, 0, 50 };
    zzi_1.linkedVal = &zziRoatate;
    zzi_1.nextInfo = &zzi_2;

    zzi_2 = { 100, -80, 0, 10 };
    zzi_2.linkedVal = &zziRoatate;
    zzi_2.nextInfo = NULL;

    body_1 = { 0, 10, 0, 50 };
    body_1.linkedVal = &bodyRotate;
    body_1.nextInfo = &body_2;

    body_2 = { 10, 0, 0, 20 };
    body_2.linkedVal = &bodyRotate;
    body_2.nextInfo = NULL;

    leg_l_1 = { 0, -20, 0, 50 };
    leg_l_1.linkedVal = &legLXRotate;
    leg_l_1.nextInfo = &leg_l_2;

    leg_l_2 = { -20, 0, 0, 20 };
    leg_l_2.linkedVal = &legLXRotate;
    leg_l_2.nextInfo = NULL;

    leg_r_1 = { 0, 20, 0, 50 };
    leg_r_1.linkedVal = &legRXRotate;
    leg_r_1.nextInfo = &leg_r_2;

    leg_r_2 = { 20, 0, 0, 20 };
    leg_r_2.linkedVal = &legRXRotate;
    leg_r_2.nextInfo = NULL;

    sunmoon_1 = { 0, 360, 0, 1000 };
    sunmoon_1.linkedVal = &sunmoonRotate;
    sunmoon_1.nextInfo = &sunmoon_1;
    enQueue(&rotateQueue, sunmoon_1);

    zzi_vib_1 = { 0, 10, 0, 15 };
    zzi_vib_1.linkedVal = &zzi_vib;
    zzi_vib_1.nextInfo = &zzi_vib_2;

    zzi_vib_2 = { 10, -10, 0, 15 };
    zzi_vib_2.linkedVal = &zzi_vib;
    zzi_vib_2.nextInfo = &zzi_vib_3;

    zzi_vib_3 = { -10, 20, 0, 15 };
    zzi_vib_3.linkedVal = &zzi_vib;
    zzi_vib_3.nextInfo = &zzi_vib_4;

    zzi_vib_4 = { 20, 0, 0, 15 };
    zzi_vib_4.linkedVal = &zzi_vib;
    zzi_vib_4.nextInfo = NULL;

    // 걷기

    arm_l_move_1 = { 0, -45, 0, 5 };
    arm_l_move_1.linkedVal = &armLXRotate;
    arm_l_move_1.nextInfo = &arm_l_move_2;
    arm_l_move_2 = { -45, 45, 0, 10 };
    arm_l_move_2.linkedVal = &armLXRotate;
    arm_l_move_2.nextInfo = &arm_l_move_3;
    arm_l_move_3 = { 45, 0, 0, 5 };
    arm_l_move_3.linkedVal = &armLXRotate;
    arm_l_move_3.nextInfo = NULL;

    arm_r_move_1 = { 100, 45, 0, 5 };
    arm_r_move_1.linkedVal = &armRXRotate;
    arm_r_move_1.nextInfo = &arm_r_move_2;
    arm_r_move_2 = { 45, -45, 0, 10 };
    arm_r_move_2.linkedVal = &armRXRotate;
    arm_r_move_2.nextInfo = &arm_r_move_3;
    arm_r_move_3 = { -45, 100, 0, 5 };
    arm_r_move_3.linkedVal = &armRXRotate;
    arm_r_move_3.nextInfo = NULL;

    leg_l_move_1 = { 0, 20, 0, 5 };
    leg_l_move_1.linkedVal = &legLXRotate;
    leg_l_move_1.nextInfo = &leg_l_move_2;
    leg_l_move_2 = { 20, -20, 0, 10 };
    leg_l_move_2.linkedVal = &legLXRotate;
    leg_l_move_2.nextInfo = &leg_l_move_3;
    leg_l_move_3 = { -20, 0, 0, 5 };
    leg_l_move_3.linkedVal = &legLXRotate;
    leg_l_move_3.nextInfo = NULL;

    leg_r_move_1 = { 0, -20, 0, 5 };
    leg_r_move_1.linkedVal = &legRXRotate;
    leg_r_move_1.nextInfo = &leg_r_move_2;
    leg_r_move_2 = { -20, 20, 0, 10 };
    leg_r_move_2.linkedVal = &legRXRotate;
    leg_r_move_2.nextInfo = &leg_r_move_3;
    leg_r_move_3 = { 20, 0, 0, 5 };
    leg_r_move_3.linkedVal = &legRXRotate;
    leg_r_move_3.nextInfo = NULL;

    // 낚기

    // 바디
    body_catch_1 = { 0, 45, 0, 100 };
    body_catch_1.linkedVal = &bodyRotate;
    body_catch_1.nextInfo = &body_catch_2;
    body_catch_2 = { 45, 0, 0, 20 };
    body_catch_2.linkedVal = &bodyRotate;
    body_catch_2.nextInfo = &body_catch_2_1;
    body_catch_2_1 = { 0, 0, 0, 100 };
    body_catch_2_1.linkedVal = &bodyRotate;
    body_catch_2_1.nextInfo = &body_catch_3;
    body_catch_3 = { 0, 0, 0, 20 };
    body_catch_3.linkedVal = &bodyRotate;
    body_catch_3.nextInfo = NULL;
    body_catch_3.canMotion = true;

    // 팔
    arm_l_x_catch_1 = { 0, 45, 0, 100 };
    arm_l_x_catch_1.linkedVal = &armLXRotate;
    arm_l_x_catch_1.nextInfo = &arm_l_x_catch_2;
    arm_l_x_catch_2 = { 45, 160, 0, 20 };
    arm_l_x_catch_2.linkedVal = &armLXRotate;
    arm_l_x_catch_2.nextInfo = &arm_l_x_catch_2_1;
    arm_l_x_catch_2_1 = { 160, 160, 0, 100 };
    arm_l_x_catch_2_1.linkedVal = &armLXRotate;
    arm_l_x_catch_2_1.nextInfo = &arm_l_x_catch_3;
    arm_l_x_catch_3 = { 160, 0, 0, 20 };
    arm_l_x_catch_3.linkedVal = &armLXRotate;
    arm_l_x_catch_3.nextInfo = NULL;

    arm_l_y_catch_1 = { 0, 45, 0, 100 };
    arm_l_y_catch_1.linkedVal = &armLYRotate;
    arm_l_y_catch_1.nextInfo = &arm_l_y_catch_2;
    arm_l_y_catch_2 = { 45, -30, 0, 20 };
    arm_l_y_catch_2.linkedVal = &armLYRotate;
    arm_l_y_catch_2.nextInfo = &arm_l_y_catch_2_1;
    arm_l_y_catch_2_1 = { -30, -30, 0, 100 };
    arm_l_y_catch_2_1.linkedVal = &armLYRotate;
    arm_l_y_catch_2_1.nextInfo = &arm_l_y_catch_3;
    arm_l_y_catch_3 = { -30, 0, 0, 20 };
    arm_l_y_catch_3.linkedVal = &armLYRotate;
    arm_l_y_catch_3.nextInfo = NULL;

    arm_r_x_catch_1 = { 45, 45, 0, 100 };
    arm_r_x_catch_1.linkedVal = &armRXRotate;
    arm_r_x_catch_1.nextInfo = &arm_r_x_catch_2;
    arm_r_x_catch_2 = { 45, 160, 0, 20 };
    arm_r_x_catch_2.linkedVal = &armRXRotate;
    arm_r_x_catch_2.nextInfo = &arm_r_x_catch_2_1;
    arm_r_x_catch_2_1 = { 160, 160, 0, 100 };
    arm_r_x_catch_2_1.linkedVal = &armRXRotate;
    arm_r_x_catch_2_1.nextInfo = &arm_r_x_catch_3;
    arm_r_x_catch_3 = { 160, 100, 0, 20 };
    arm_r_x_catch_3.linkedVal = &armRXRotate;
    arm_r_x_catch_3.nextInfo = NULL;

    arm_r_y_catch_1 = { 0, -30, 0, 100 };
    arm_r_y_catch_1.linkedVal = &armRYRotate;
    arm_r_y_catch_1.nextInfo = &arm_r_y_catch_2;
    arm_r_y_catch_2 = { -30, 30, 0, 20 };
    arm_r_y_catch_2.linkedVal = &armRYRotate;
    arm_r_y_catch_2.nextInfo = &arm_r_y_catch_2_1;
    arm_r_y_catch_2_1 = { 30, 30, 0, 100 };
    arm_r_y_catch_2_1.linkedVal = &armRYRotate;
    arm_r_y_catch_2_1.nextInfo = &arm_r_y_catch_3;
    arm_r_y_catch_3 = { 30, 0, 0, 20 };
    arm_r_y_catch_3.linkedVal = &armRYRotate;
    arm_r_y_catch_3.nextInfo = NULL;

    // 다리
    leg_l_x_catch_1 = { 0, -20, 0, 100 };
    leg_l_x_catch_1.linkedVal = &legLXRotate;
    leg_l_x_catch_1.nextInfo = &leg_l_x_catch_2;
    leg_l_x_catch_2 = { -20, 10, 0, 20 };
    leg_l_x_catch_2.linkedVal = &legLXRotate;
    leg_l_x_catch_2.nextInfo = &leg_l_x_catch_2_1;
    leg_l_x_catch_2_1 = { 10, 10, 0, 100 };
    leg_l_x_catch_2_1.linkedVal = &legLXRotate;
    leg_l_x_catch_2_1.nextInfo = &leg_l_x_catch_3;
    leg_l_x_catch_3 = { 10, 0, 0, 20 };
    leg_l_x_catch_3.linkedVal = &legLXRotate;
    leg_l_x_catch_3.nextInfo = NULL;

    leg_l_y_catch_1 = { 0, 20, 0, 100 };
    leg_l_y_catch_1.linkedVal = &legLYRotate;
    leg_l_y_catch_1.nextInfo = &leg_l_y_catch_2;
    leg_l_y_catch_2 = { 20, 20, 0, 20 };
    leg_l_y_catch_2.linkedVal = &legLYRotate;
    leg_l_y_catch_2.nextInfo = &leg_l_y_catch_2_1;
    leg_l_y_catch_2_1 = { 20, 20, 0, 100 };
    leg_l_y_catch_2_1.linkedVal = &legLYRotate;
    leg_l_y_catch_2_1.nextInfo = &leg_l_y_catch_3;
    leg_l_y_catch_3 = { 20, 0, 0, 20 };
    leg_l_y_catch_3.linkedVal = &legLYRotate;
    leg_l_y_catch_3.nextInfo = NULL;

    leg_r_x_catch_1 = { 0, -20, 0, 100 };
    leg_r_x_catch_1.linkedVal = &legRXRotate;
    leg_r_x_catch_1.nextInfo = &leg_r_x_catch_2;
    leg_r_x_catch_2 = { -20, 10, 0, 20 };
    leg_r_x_catch_2.linkedVal = &legRXRotate;
    leg_r_x_catch_2.nextInfo = &leg_r_x_catch_2_1;
    leg_r_x_catch_2_1 = { 10, 10, 0, 100 };
    leg_r_x_catch_2_1.linkedVal = &legRXRotate;
    leg_r_x_catch_2_1.nextInfo = &leg_r_x_catch_3;
    leg_r_x_catch_3 = { 10, 0, 0, 20 };
    leg_r_x_catch_3.linkedVal = &legRXRotate;
    leg_r_x_catch_3.nextInfo = NULL;

    leg_r_y_catch_1 = { 0, -20, 0, 100 };
    leg_r_y_catch_1.linkedVal = &legRYRotate;
    leg_r_y_catch_1.nextInfo = &leg_r_y_catch_2;
    leg_r_y_catch_2 = { -20, -50, 0, 20 };
    leg_r_y_catch_2.linkedVal = &legRYRotate;
    leg_r_y_catch_2.nextInfo = &leg_r_y_catch_2_1;
    leg_r_y_catch_2_1 = { -50, -50, 0, 100 };
    leg_r_y_catch_2_1.linkedVal = &legRYRotate;
    leg_r_y_catch_2_1.nextInfo = &leg_r_y_catch_3;
    leg_r_y_catch_3 = { -50, 0, 0, 20 };
    leg_r_y_catch_3.linkedVal = &legRYRotate;
    leg_r_y_catch_3.nextInfo = NULL;

    zzi_catch_1 = { -80, -115, 0, 100 };
    zzi_catch_1.linkedVal = &zziRoatate;
    zzi_catch_1.nextInfo = &zzi_catch_2;
    zzi_catch_2 = { -115, -150, 0, 20 };
    zzi_catch_2.linkedVal = &zziRoatate;
    zzi_catch_2.nextInfo = &zzi_catch_2_1;
    zzi_catch_2_1 = { -150, -150, 0, 100 };
    zzi_catch_2_1.linkedVal = &zziRoatate;
    zzi_catch_2_1.nextInfo = &zzi_catch_3;
    zzi_catch_3 = { -150, 0, 0, 20 };
    zzi_catch_3.linkedVal = &zziRoatate;
    zzi_catch_3.nextInfo = NULL;

}

void InitLight() {
    GLfloat light0_ambient[] = { 0.5, 0.4, 0.3, 1.0 };     //해
    GLfloat light0_diffuse[] = { 0.8, 0.7, 0.6, 1.0 };
    GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    GLfloat light1_ambient[] = { 0.001, 0.001, 0.001, 1.0 };     //달
    GLfloat light1_diffuse[] = { 0.002, 0.002, 0.002, 1.0 };
    GLfloat light1_specular[] = { 0.005, 0.005, 0.005, 1.0 };

    glShadeModel(GL_SMOOTH);    //구로 셰이딩
    glEnable(GL_DEPTH_TEST);    //깊이 버퍼 활성화
    glEnable(GL_LIGHTING);      //조명 활성화
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_ambient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_ambient);
}

void InitMetari() {
    GLfloat mat_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
    GLfloat mat_specular[] = { 0, 0, 0, 1.0 };
    GLfloat mat_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat mat_shininess[] = { 0.5 };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void drawRealFish() {
    glPushMatrix(); // 몸통
        glColor3f(c(63), c(72), c(204));
        glScalef(1.5, 0.5, 1);
        glutSolidSphere(1, 20, 20);

        glPushMatrix(); // 눈
            glColor3f(0, 0, 0);
            glTranslatef(-0.6, 0.7, 0.5);
            glScalef(0.1, 0.3, 0.125);
            glutSolidSphere(1, 10, 10);
        glPopMatrix();

        glPushMatrix(); // 지느러미
            glColor3f(1, 1, 1);
            glTranslatef(0, 0, 1);
            glScalef(0.5, 0.5, 0.25);
            glutSolidCone(1, 1, 10, 10);
        glPopMatrix();

        glPushMatrix(); // 꼬리
            glColor3f(1, 1, 1);
            glTranslatef(1.5, 0, 0);
            glRotatef(-90, 0, 1, 0);
            glScalef(1, 0.5, 1);
            glutSolidCone(1, 1, 10, 10);
        glPopMatrix();

    glPopMatrix();
}

void drawFishingRod() {
    glColor3f(c(150), c(90), c(0));

    glPushMatrix();
        // 낚싯대
        glRotatef(90, 1, 0, 0);
        gluCylinder(p, 0.1, 0.1, 3, 6, 6);
        glTranslatef(0, 0, 3); // 낚시대 앞 부분으로 이동

        glPushMatrix(); // 낚싯줄
            glRotatef(zziRoatate, 1, 0, 0);
            if (motioning || fishing) {
                glColor3f(0, 0, 0);
                gluCylinder(p, 0.05, 0.05, 8, 6, 6);

                glPushMatrix(); //찌
                    glColor3f(1, 0, 0);
                    glTranslatef(0, 0, 8);
                    glRotatef(-50, 1, 0, 0);
                    glTranslatef(0, 0, zzi_vib / 10);
                    glutSolidSphere(zziSize, 10, 10);

                    if (catching) {
                        glPushMatrix(); // 물고기
                            glRotatef(225, 1, 0, 1);
                            glColor3f(catchR, catchG, catchB);

                            switch (catchFish) {
                                case Sphere:
                                    glutSolidSphere(1, 10, 10);
                                    break;
                                case Cube:
                                    glutSolidCube(1);
                                    break;
                                case Torus:
                                    glutSolidTorus(0.5, 1, 10, 10);
                                    break;
                                case Cone:
                                    glutSolidCone(1, 1, 10, 10);
                                    break;
                                case Icosahedron:
                                    glutSolidCube(1);
                                    break;
                                case Teapot:
                                    glutWireTeapot(1);
                                    break;
                                case RealFish:
                                    drawRealFish();
                                break;
                            }
                        glPopMatrix();
                    }
                glPopMatrix();
            }
        glPopMatrix();
    glPopMatrix();
}

void drawPlayer() {
    glColor3f(1, 1, 1);

    glPushMatrix(); // 원점 설정 및 이동
        glTranslatef(0, 0, 0);
        glTranslatef(moveX, 0, moveY);
        glRotatef(rotate, 0, 1, 0);

        //몸통
        glTranslatef(0, 3, 0);
        glRotatef(90 + bodyRotate, 1.0, 0, 0);
        gluCylinder(p, 0.5, 0.5, 1, 10, 10);

        glPushMatrix(); // 머리
            glTranslatef(0, 0, -1);
            glutSolidSphere(1, 10, 10);
        glPopMatrix();

        glPushMatrix(); // 왼팔
            glTranslatef(-0.7, 0, 0);
            glRotatef(armLXRotate, 1, 0, 0);
            glRotatef(armLYRotate, 0, 1, 0);
            gluCylinder(p, 0.2, 0.2, 1, 10, 10);
        glPopMatrix();

        glPushMatrix(); // 오른팔
            glTranslatef(0.7, 0, 0);
            glRotatef(armRXRotate, 1, 0, 0);
            glRotatef(armRYRotate, 0, 1, 0);
            gluCylinder(p, 0.2, 0.2, 1, 10, 10);

            glPushMatrix(); // 손
                glTranslatef(0, 0, 0.9);
                drawFishingRod();
                glColor3f(1, 1, 1);
            glPopMatrix();
        glPopMatrix();

        glPushMatrix(); // 다리1
            glTranslatef(0.2, 0, 1);
            glRotatef(legLXRotate, 1, 0, 0);
            glRotatef(legLYRotate, 0, 1, 0);
            gluCylinder(p, 0.2, 0.2, 1, 10, 10);
        glPopMatrix();

        glPushMatrix(); // 다리2
            glTranslatef(-0.2, 0, 1);
            glRotatef(legRXRotate, 1, 0, 0);
            glRotatef(legRYRotate, 0, 1, 0);
            gluCylinder(p, 0.2, 0.2, 1, 10, 10);
        glPopMatrix();
    glPopMatrix();
}

void drawIsland() {
    glColor3f(c(255), c(218), c(119));
    glPushMatrix();
    glScalef(10, 1, 10);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();
}

void drawSea() {
    glColor3f(c(0), c(128), c(255));
    glPushMatrix();
    glScalef(100, 0.1, 100);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();
}

void drawSunAndMoon() {
    GLfloat LightPosition1[] = { 0.0, 20, 0.0, 1.0 };
    GLfloat LightPosition2[] = { 0.0, -20, 0.0, 1.0 };

    glPushMatrix();
        glRotatef(sunmoonRotate, 0.5, 0.0, 0.5);
        
        glLightfv(GL_LIGHT0, GL_POSITION, LightPosition1);
        glLightfv(GL_LIGHT1, GL_POSITION, LightPosition2);

        glDisable(GL_LIGHTING);

        glPushMatrix();
            glColor3f(1, 0, 0);
            glTranslatef(0, 20, 0);
            glutSolidSphere(1, 10, 10);
        glPopMatrix();

        glPushMatrix();
            glColor3f(1, 1, 0);
            glTranslatef(0, -20, 0);
            glutSolidSphere(1, 10, 10);
        glPopMatrix();

        glEnable(GL_LIGHTING);
    glPopMatrix();
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawSea();
    drawIsland();
    drawPlayer();
    drawSunAndMoon();
    glFlush();
}

void motionInfoHandler() {
    while (!isEmpty(rotateQueue)) {
        MotionInfo tmpInfo = deQueue(&rotateQueue);
        if (tmpInfo.tick == 0) {
            tmpInfo.val = tmpInfo.start;
        }
        else if (tmpInfo.tick == tmpInfo.maxTick) {
            tmpInfo.val = tmpInfo.end;
        }
        else {
            if (tmpInfo.start <= tmpInfo.end) {
                tmpInfo.val = tmpInfo.start + ((tmpInfo.end - tmpInfo.start) * ((tmpInfo.tick) / (float)tmpInfo.maxTick));
            }
            else {
                tmpInfo.val = tmpInfo.start - ((tmpInfo.start - tmpInfo.end) * ((tmpInfo.tick) / (float)tmpInfo.maxTick));
            }
        }

        if (tmpInfo.tick == tmpInfo.maxTick) {
            if (tmpInfo.nextInfo != NULL) {
                enQueue(&rotateQueueTmp, *(tmpInfo.nextInfo));
            }
            if (tmpInfo.canMotion) {
                motioning = false;
                catching = false;
            }
        }
        else {
            tmpInfo.tick++;
            enQueue(&rotateQueueTmp, tmpInfo);
        }

        *tmpInfo.linkedVal = tmpInfo.val;
    }
    memcpy(&rotateQueue, &rotateQueueTmp, sizeof(struct Queue));
    queueInit(&rotateQueueTmp);
}

void MyTimer(int Value) {
    if (fishing) {
        if (rand() % 1000 <= 1) {
            zziSaveTime = 100;
            enQueue(&rotateQueue, zzi_vib_1);
            hit = true;
        }
        else if (zziSaveTime > 0) {
            zziSaveTime--;
        }
        else if(zziSaveTime <= 0) {
            hit = false;
        }
    }
    motionInfoHandler();
    glutPostRedisplay();
    glutTimerFunc(10, MyTimer, 1);
}

void MyReshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-15.0, 15.0, -15.0, 15.0, -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(MX / 300.0, MY / 200, 1.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);  //시점변환
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
    if (!motioning && !fishing) { // 이동 관련
        float tmpX = moveX, tmpY = moveY;
        switch (KeyPressed) {
            case 'w':
            case 'W':
                moveX -= 0.5;
                moveY -= 0.5;
                rotate = 45;
                enQueue(&rotateQueue, arm_l_move_1);
                enQueue(&rotateQueue, arm_r_move_1);
                enQueue(&rotateQueue, leg_l_move_1);
                enQueue(&rotateQueue, leg_r_move_1);
                break;
            case 'a':
            case 'A':
                moveX -= 0.5;
                moveY += 0.5;
                rotate = 135;
                enQueue(&rotateQueue, arm_l_move_1);
                enQueue(&rotateQueue, arm_r_move_1);
                enQueue(&rotateQueue, leg_l_move_1);
                enQueue(&rotateQueue, leg_r_move_1);
                break;
            case 's':
            case 'S':
                moveX += 0.5;
                moveY += 0.5;
                rotate = 225;
                enQueue(&rotateQueue, arm_l_move_1);
                enQueue(&rotateQueue, arm_r_move_1);
                enQueue(&rotateQueue, leg_l_move_1);
                enQueue(&rotateQueue, leg_r_move_1);
                break;
            case 'd':
            case 'D':
                moveX += 0.5;
                moveY -= 0.5;
                rotate = 315;
                enQueue(&rotateQueue, arm_l_move_1);
                enQueue(&rotateQueue, arm_r_move_1);
                enQueue(&rotateQueue, leg_l_move_1);
                enQueue(&rotateQueue, leg_r_move_1);
                break;
        }
        if (moveX * moveX + moveY * moveY >= 100) {
            moveX = tmpX;
            moveY = tmpY;
        }
    }

    // 모션 관련
    switch (KeyPressed) {
        case 'q':
        case 'Q':
            testrotate--;
            break;
        case 'e':
        case 'E':
            testrotate++;
            break;
        case VK_SPACE:
            if (fishing) {
                motioning = true;
                fishing = false;
                enQueue(&rotateQueue, arm_l_x_catch_1);
                enQueue(&rotateQueue, arm_l_y_catch_1);
                enQueue(&rotateQueue, arm_r_x_catch_1);
                enQueue(&rotateQueue, arm_r_y_catch_1);

                enQueue(&rotateQueue, leg_l_x_catch_1);
                enQueue(&rotateQueue, leg_l_y_catch_1);
                enQueue(&rotateQueue, leg_r_x_catch_1);
                enQueue(&rotateQueue, leg_r_y_catch_1);

                enQueue(&rotateQueue, body_catch_1);
                enQueue(&rotateQueue, zzi_catch_1);

                if (hit) {
                    catching = true;
                    catchR = c(rand() % 255);
                    catchG = c(rand() % 255);
                    catchB = c(rand() % 255);
                    catchFish = (Fish)(rand() % FishMax);
                    printf("잡았다!\n");
                }
                else {
                    printf("실패했다..\n");
                }
                hit = false;
            }
            else if (!motioning) {
                motioning = true;
                fishing = true;
                enQueue(&rotateQueue, arm_1);
                enQueue(&rotateQueue, zzi_1);
                enQueue(&rotateQueue, body_1);
                enQueue(&rotateQueue, leg_l_1);
                enQueue(&rotateQueue, leg_r_1);
            }
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    dataInit();
    glutInit(&argc, argv);
    
    glutInitWindowSize(600, 600);
    glutCreateWindow("Fishing");
    
    p = gluNewQuadric();
    gluQuadricDrawStyle(p, GLU_FILL);

    glClearColor(0, 0, 0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glutReshapeFunc(MyReshape);
    glutTimerFunc(10, MyTimer, 1);
    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboard);
    InitLight();
    InitMetari();

    glutMainLoop();
    return 0;
}