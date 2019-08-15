#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <conio.h>  //kbhit()ʹ��

int score = 0;//����
int rank = 0;//�ȼ�

#define BLOCK_COUNT  5  //���������
#define BLOCK_WIDTH  5  //��� ->��(X��)
#define BLOCK_HEIGHT 5  //�߶�->�У�Y�ᣩ
#define UNIT_SIZE    20  //С����ռ������

#define START_X      130  //�궨����ʼ���
#define START_Y      30   //�궨����ʼ�߶�

#define KEY_UP      72
#define KEY_RIGHT   77
#define KEY_DOWN	80
#define KEY_LEFT    75
#define KEY_SPACE   32

int speed = 500;//500����
int minX = 30;
int minY = 30;

typedef enum{
	BLOCK_UP,
	BLOCK_RIGHT,
	BLOCK_DOWN,
	BLOCK_LEFT
}block_dir_t; //���鳯��

typedef enum{  //�˶�����
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
}move_dir_t;

int NextIndex = -1;//��һ������
int BlockIndex = -1;//��ǰ��Ϸ���﷽�������

int color[BLOCK_COUNT] = {
	GREEN,CYAN,MAGENTA,BROWN,YELLOW
};  //������ֱ��ʾÿһ�ַ������ɫ

int visit[30][15];//�������飬������Ϸ�����Ϊ30��15��
int markColor[30][15];//��ʾ��Ӧλ�õ���ɫ

int block[BLOCK_COUNT*4][BLOCK_HEIGHT][BLOCK_WIDTH]={
	//���η���---------------------------------1
	{
		0,0,0,0,0,
		0,0,1,0,0,
		0,0,1,0,0,
		0,0,1,0,0,
		0,0,0,0,0,
	},

	{
		0,0,0,0,0,
		0,0,0,0,0,
		0,1,1,1,0,
		0,0,0,0,0,
		0,0,0,0,0,	
	},

	{
		0,0,0,0,0,
		0,0,0,0,0,
		0,1,1,1,0,
		0,0,0,0,0,
		0,0,0,0,0,		
	},

	{
		0,0,0,0,0,
		0,0,1,0,0,
		0,0,1,0,0,
		0,0,1,0,0,
		0,0,0,0,0,
	},

	//L �η���-------------------------------2
	{
		0,0,0,0,0,
		0,0,1,0,0,
		0,0,1,0,0,
		0,0,1,1,0,
		0,0,0,0,0,
	},

	{
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,0,1,0,
		0,1,1,1,0,
		0,0,0,0,0,	
	},

	{
		0,0,0,0,0,
		0,1,1,0,0,
		0,0,1,0,0,
		0,0,1,0,0,
		0,0,0,0,0,		
	},

	{
		0,0,0,0,0,
		0,0,0,0,0,
		0,1,1,1,0,
		0,1,0,0,0,
		0,0,0,0,0,
	},

	//T �η���----------------------------3
	{
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,1,0,0,
		0,1,1,1,0,
		0,0,0,0,0,
	},

	{
		0,0,0,0,0,
		0,0,1,0,0,
		0,0,1,1,0,
		0,0,1,0,0,
		0,0,0,0,0,	
	},

	{
		0,0,0,0,0,
		0,0,0,0,0,
		0,1,1,1,0,
		0,0,1,0,0,
		0,0,0,0,0,		
	},

	{
		0,0,0,0,0,
		0,0,1,0,0,
		0,1,1,0,0,
		0,0,1,0,0,
		0,0,0,0,0,
	},

	//���η���---------------------------4
	{
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,1,1,0,
		0,0,1,1,0,
		0,0,0,0,0,
	},

	{
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,1,1,0,
		0,0,1,1,0,
		0,0,0,0,0,
	},

	{
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,1,1,0,
		0,0,1,1,0,
		0,0,0,0,0,
	},

	{
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,1,1,0,
		0,0,1,1,0,
		0,0,0,0,0,
	},

	//Z����-------------------------------5
	{
		0,0,0,0,0,
		0,0,0,0,0,
		0,1,1,0,0,
		0,0,1,1,0,
		0,0,0,0,0,
	},

	{
		0,0,0,0,0,
		0,0,0,0,0,
		0,0,1,1,0,
		0,1,1,0,0,
		0,0,0,0,0,	
	},

	{
		0,0,0,0,0,
		0,0,1,0,0,
		0,1,1,0,0,
		0,1,0,0,0,
		0,0,0,0,0,		
	},

	{
		0,0,0,0,0,
		0,0,1,0,0,
		0,0,1,1,0,
		0,0,0,1,0,
		0,0,0,0,0,
	}
};

//��ʼ����ӭ����
void welcome(void){
	//���ó�ʼ�������С
	initgraph(550,660);
	//���ñ�����
	HWND window = GetHWnd();//��ȡ��ǰ����
	SetWindowText(window,_T("����˹����--̷"));//ָ���������ñ���

	//�����ı�������ʽ
	setfont(40,0,_T("����"));//�߶ȣ����
	//����������ɫ
	setcolor(BROWN);
	//ָ��λ��������壨���ݣ�
	outtextxy(205,200,_T("����˹����"));

	setfont(22,0,_T("����"));
	outtextxy(175,300,_T("���뾭�䡪������˹����"));

	Sleep(3000);//����˯�ߣ���ͣ��3000���룬3��
}

//��ʼ����Ϸ����
void initGameScene(void){
	char str[16];//�������
	//�����Ļ����ӭ���棩
	cleardevice();
	//����Ϸ�����򣨽�����Ϸ�ķ���
	rectangle(27,27,336,635);//ǰ��������Ϊ���Ͻ����꣬��Ϊ���½�����
	rectangle(29,29,334,633);
	//�������Ͻ�С����
	rectangle(370,50,515,195);

	//�������½������������ԣ��������ȼ���
	setfont(24,0,_T("����"));
	setcolor(BLUE);
	outtextxy(390,215,_T("��һ������"));

	setcolor(RED);
	outtextxy(405,280,_T("����"));
	
	//��ָ����ʽ���ַ�����ӡ���ַ�������
	sprintf(str,"%d",score);//��������ӡ���ַ�����str��
	outtextxy(415,310,str);

	outtextxy(405,375,_T("�ȼ�"));
	sprintf(str,"%d",rank);
	outtextxy(425,405,str);

	//����˵��
	setcolor(LIGHTBLUE);
	outtextxy(390,475,_T("����˵��"));
	outtextxy(390,500,"��:��ת");
	outtextxy(390,525,"��:�����½�");
	outtextxy(390,550,"��:����");
	outtextxy(390,575,"��:����");
	outtextxy(390,600,"�ո�:��ͣ");

}

//����С�����
void clearBlock(void){
	setcolor(BLACK);
	setfont(23,0,"����");//�����Ĵ�С����һ��

	for(int i = 0;i<BLOCK_HEIGHT;i++){  //�ڼ���
		for(int j = 0;j<BLOCK_WIDTH;j++){  //�ڼ���
			int x = 391 + j * UNIT_SIZE;  //��ʼ���꣬��ʾ����
			int y = 71 + i * UNIT_SIZE;
			outtextxy(x,y,"��");//"��"��ָ��λ�ô�ӡ
		}
	}
}

//�����Ͻ�С�����ڣ�����һ������
void drawBlock(int x,int y){
	setcolor(color[NextIndex]);//������ɫ�������÷�����ɫ
	setfont(23,0,"����");//�����Ĵ�С����һ��

	for(int i = 0;i<BLOCK_HEIGHT;i++){  //�ڼ���
		for(int j = 0;j<BLOCK_WIDTH;j++){  //�ڼ���
			if(block[NextIndex*4][i][j] == 1){		//�ж��Ƿ�Ϊ1��Ϊ1�ͻ�
			int x1 = x + j * UNIT_SIZE;  //��ʼ���꣬��ʾ����
			int y1 = y + i * UNIT_SIZE;
			outtextxy(x1,y1,"��");//"��"��ָ��λ�ô�ӡ
			}
		}
	}
}

//ָ��λ�û���ָ�������ָ������
void drawBlock(int x,int y,int blockIndex,block_dir_t dir){
	setcolor(color[blockIndex]);//������ɫ�������÷�����ɫ
	setfont(23,0,"����");//�����Ĵ�С����һ��
	int id = blockIndex*4+dir;
	for(int i = 0;i<BLOCK_HEIGHT;i++){  //�ڼ���
		for(int j = 0;j<BLOCK_WIDTH;j++){  //�ڼ���
			if(block[id][i][j] == 1){		//�ж��Ƿ�Ϊ1��Ϊ1�ͻ�
			int x1 = x + j * UNIT_SIZE;  //��ʼ���꣬��ʾ����
			int y1 = y + i * UNIT_SIZE;
			outtextxy(x1,y1,"��");//"��"��ָ��λ�ô�ӡ
			}
		}
	}
}

//���ָ��λ��ָ������ķ���
//����x����������Ͻǵ�x����
//����y:�������Ͻ�����Ϸ���ڵ����������Ϸ����������
void drawBlock(int x,int y,block_dir_t dir){
	setcolor(BLACK);//�������飨��ɫ������
	int id = BlockIndex * 4 + dir;
	y+=START_Y;
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			if(block[id][i][j] == 1){
				//�����÷���ĵ�i�еĵ�j��
				outtextxy(x+j*20,y+i*20,"��");
			}
		}
	}
}

void clearBlock(int x,int y,block_dir_t dir){
	setcolor(BLACK);//�������飨��ɫ������
	int id = BlockIndex * 4 + dir;
	y+=START_Y;
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			if(block[id][i][j] == 1){
				//�����÷���ĵ�i�еĵ�j��
				outtextxy(x+20*j,y+i*20,"��");
			}
		}
	}
}

//�����·���
void nextblock(void){
	//���С�����ڵ�����
	clearBlock();

	//���ѡ��һ�ַ��飨���������ʾ��
	srand(time(NULL));//ʹ��ʱ�亯������ֵ��Ϊ�������
	NextIndex = rand() % BLOCK_COUNT;//����ȡ�࣬����������Ʒ�Χ

	//����һ������ǰ�������ĺ���,��С�����ڻ�
	drawBlock(391,71);
}

//�����ָ��λ�ÿ�����ָ�������ƶ��򷵻�1������Ϊ0
int moveable(int x1,int y1,move_dir_t moveDir,block_dir_t blockDir){
	//���㵱ǰ��������Ͻ���30*15����Ϸ���е�λ��(�ڼ���,�ڼ���)
	int x = (y1 - minY) / UNIT_SIZE;
	int y = (x1 - minX) / UNIT_SIZE;
	int id = BlockIndex * 4 + blockDir; //����ڼ���(��)����
	int ret = 1;//1���˶���0����

	if(moveDir == MOVE_DOWN){
		for(int i = 0; i<5;i++){
			for(int j = 0;j<5;j++){
				if(block[id][i][j] == 1 && 
					(x + i + 1>= 30 || visit[x+i+1][y+j]==1)){
						ret = 0;
				}
			}
		}//�������ж��Ƿ���Ļ�ʵ��
	}else if(moveDir == MOVE_LEFT){
		for(int i = 0; i<5;i++){
			for(int j = 0;j<5;j++){
				if(block[id][i][j] == 1 && 
					(y + j == 0 || visit[x+i][y+j-1]==1)){
						ret = 0;
				}
			}
		}
	}else if(moveDir == MOVE_RIGHT){
		for(int i = 0;i<5;i++){
			for(int j = 0;j<5;j++){
				if(block[id][i][j] == 1 && 
					(y+j+1>=15 || visit[x+i][y+j+1]==1)){
						ret = 0;
				}
			}
		}
	}

	return ret;
}

//�����Ϸ�Ƿ����
void failCheck(){
	if(!moveable(START_X,START_Y,MOVE_DOWN,BLOCK_UP)){
		setcolor(WHITE);
		setfont(45,0,"����");//��������
		outtextxy(75,300,"Ŷ��!������!");//���
		Sleep(1000);  //����1��
		system("pause");
		closegraph();  //�ر�ͼ�����
		exit(0); //�����������
	}
}

//���߹���
void wait(int interval){
	int count = interval / 10;//���ߴ���
	for(int i=0;i<count;i++){
		Sleep(10);
		if(kbhit()){
			return;
		}
	}
}

//�жϵ�ǰ�����Ƿ����ת��ָ������
//��ʱֻ���жϻ�û��ת
int rotatable(int x,int y,block_dir_t dir){
	int id = BlockIndex * 4 + dir;
	int xIndex = (y-minY) / 20;//**����3**
	int yIndex = (x-minX) / 20;

	if(!moveable(x,y,MOVE_DOWN,dir)){
		return 0;
	}//�жϷ����ܷ����¹���
 
	for(int i=0;i<5;i++){
		for(int j = 0;j<5;j++){
			if(block[id][i][j] == 1 &&
				(yIndex+j<0 || yIndex+j>=15 || visit[xIndex+i][yIndex+j]==1)){
				return 0;
			}//�жϱ߽�ȷ����ת
		}
	}
	return 1;	
}

//���鵽��߽���̶� 
void mark(int x,int y,int blockIndex,block_dir_t dir){
	int id = blockIndex * 4 +dir;
	int x2 = (y - minY) / 20;
	int y2 = (x - minX) / 20;
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			if(block[id][i][j] == 1){
				visit[x2+i][y2+j]=1;
				markColor[x2+i][y2+j] = color[blockIndex];
			}
		}
	}
}

void move(void){
	int x = START_X;
	int y = START_Y;
	int k = 0;
	block_dir_t blockDir = BLOCK_UP; //����ĳ���
	int curSpeed = speed;

	//�����Ϸ�Ƿ����
	failCheck();

	//����������
	while(1){
		
		//����ʱ�˿���û�а�������ȥ,����ȥ����1
		if(kbhit()){
			int key = getch();  //����
			if(key == KEY_SPACE){   //if�����ж��á�==��**����1**
				getch();
			}
		}

		//�����ǰ����
		clearBlock(x,k,blockDir);   //blockDir���鳯��

		//�жϰ�����û�а���ȥ
		if(kbhit()){
			int key = getch();
			
			if(key == KEY_UP){    //������ת
				block_dir_t nextDir = (block_dir_t)((blockDir + 1) % 4);//��һ������
				if(rotatable(x,y+k,nextDir)){
					blockDir = nextDir;
				}
			}else if(key == KEY_DOWN){
				curSpeed = 50;
			}else if(key == KEY_LEFT){
				if(moveable(x,y+k+20,MOVE_LEFT,blockDir)){
					x -= 20;			//һֱ��������**����4**
				}
			}else if(key == KEY_RIGHT){
				if(moveable(x,y+k+20,MOVE_RIGHT,blockDir)){
					x += 20;          //ͬ�����4 **����5**
				}
			}
		}

		k += 20;

		//���ݷ������ָ��,���Ƶ�ǰ����
		drawBlock(x,y+k,BlockIndex,blockDir);

		wait(curSpeed);//�ȴ�һ��ʱ�䣬�ɱ䣬Sleep(curSleep)Ҳ�����ã��в�ͬ��������
		
		//k += 20;��λ�ò���   **����2**

		//����Ĺ̻�����,"����"
		if(!moveable(x,y+k,MOVE_DOWN,blockDir)){
			mark(x,y+k,BlockIndex,blockDir);
			break;
		}
	}
}

//�����·���
void newblock(void){
	//ȷ������ʹ�õķ�������
	BlockIndex = NextIndex;

	//���ƸմӶ����½��ķ���
	drawBlock(START_X,START_Y);//���ƶ������䷽��

	//���³��ֵķ�����ͣһ��
	Sleep(100);//0.1��

	//�����Ͻ����򣬻�����һ������
	nextblock();

	//���������ƶ������䣩
	move();

}

void down(int x){
	for(int i=x;i>0;i--){
		//������i�У���j�еķ�������
		for(int j=0;j<15;j++){
			if(visit[i-1][j]){
				visit[i][j] = 1;
				markColor[i][j] = markColor[i-1][j];
				setcolor(markColor[i][j]);//������ɫ
				outtextxy(20*j+minX,20*i+minY,"��");
			}else{
				visit[i][j] = 0;
				setcolor(BLACK);
				outtextxy(20*j+minX,20*i+minY,"��");
			}
		}
	}
	//�����ϵ�һ�У����б�Ϊ0��һ�У�
	setcolor(BLACK);
	for(int j=0;j<15;j++){
		visit[0][j] = 0;
		outtextxy(20*j+minX,minY,"��");
	}
}

//���·�����lines��ʾ����������
void addScore(int lines){
	char str[32];

	setcolor(RED);
	score += lines * 10;//����
	sprintf(str,"%d",score);
	outtextxy(415,310,str);
}

//�ȼ�����
void updateGrade(){
	//���µȼ��ı�ʾ,80��1��
	rank = score / 80;
	char str[16];
	sprintf(str,"%d",rank);
	outtextxy(425,405,str);
	//�����ٶ�,�ȼ�Խ���ٶ�Խ�죬˯�߾�Խ��
	//����500�����100�����룩
	speed = 500 - rank *100;
	if(speed <= 100){
		speed = 100;
	}
}

//��������
void check(void){
	int i,j;
	int clearLines = 0;//ÿ����һ�еķ���
	for(i=29;i>=0;i--){
		//����i����û����
		for(j=0;j<15 && visit[i][j];j++);
			
			//ִ�е��˴�ʱ�������������
			//1.��i��û�������п�λ����ʱj<15
			//2.��i���Ѿ����ˣ���ʱj>=15
			if(j>=15){
				//��ʱ��i����������������i��
				down(i);//������i�У���������������
				i++;   //��Ϊ������ѭ������i--��������i++,ʹ�´�ѭ��ʱ���ٰ���һ�м��һ��
				clearLines++;
		}
	}
	//���·���
	addScore(clearLines);

	//���µȼ�--�����ٶ�
	updateGrade();
}

int main(void){
	welcome();//��ӭ����
	initGameScene();//��Ϸ����
	
	nextblock();//�����·���
	Sleep(500);  //��ͣ500����
	memset(visit,0,sizeof(visit));//��ʼ����������,ȫ���ֵΪ0



	while(1){			//��Ϸѭ��
		newblock();

		//���֣��������У������·������ٶȣ�
		check();
	}
	
	system("pause");//������ͣ
	closegraph();//�رս���
	return 0;
}