#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <conio.h>  //kbhit()使用

int score = 0;//分数
int rank = 0;//等级

#define BLOCK_COUNT  5  //方块的种类
#define BLOCK_WIDTH  5  //宽度 ->列(X轴)
#define BLOCK_HEIGHT 5  //高度->行（Y轴）
#define UNIT_SIZE    20  //小方块占的像素

#define START_X      130  //宏定义起始宽度
#define START_Y      30   //宏定义起始高度

#define KEY_UP      72
#define KEY_RIGHT   77
#define KEY_DOWN	80
#define KEY_LEFT    75
#define KEY_SPACE   32

int speed = 500;//500毫秒
int minX = 30;
int minY = 30;

typedef enum{
	BLOCK_UP,
	BLOCK_RIGHT,
	BLOCK_DOWN,
	BLOCK_LEFT
}block_dir_t; //方块朝向

typedef enum{  //运动方向
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
}move_dir_t;

int NextIndex = -1;//下一个方块
int BlockIndex = -1;//当前游戏区里方块的种类

int color[BLOCK_COUNT] = {
	GREEN,CYAN,MAGENTA,BROWN,YELLOW
};  //用数组分别表示每一种方块的颜色

int visit[30][15];//访问数组，将主游戏界面分为30行15列
int markColor[30][15];//表示对应位置的颜色

int block[BLOCK_COUNT*4][BLOCK_HEIGHT][BLOCK_WIDTH]={
	//条形方块---------------------------------1
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

	//L 形方块-------------------------------2
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

	//T 形方块----------------------------3
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

	//田形方块---------------------------4
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

	//Z字形-------------------------------5
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

//初始化欢迎界面
void welcome(void){
	//设置初始化界面大小
	initgraph(550,660);
	//设置标题栏
	HWND window = GetHWnd();//获取当前窗口
	SetWindowText(window,_T("俄罗斯方块--谭"));//指定窗口设置标题

	//设置文本字体样式
	setfont(40,0,_T("宋体"));//高度，宽度
	//设置字体颜色
	setcolor(BROWN);
	//指定位置输出字体（内容）
	outtextxy(205,200,_T("俄罗斯方块"));

	setfont(22,0,_T("楷体"));
	outtextxy(175,300,_T("进入经典——俄罗斯方块"));

	Sleep(3000);//界面睡眠（暂停）3000毫秒，3秒
}

//初始化游戏场景
void initGameScene(void){
	char str[16];//分数存放
	//清除屏幕（欢迎界面）
	cleardevice();
	//画游戏主方框（进行游戏的方框）
	rectangle(27,27,336,635);//前两个参数为左上角坐标，后为右下角坐标
	rectangle(29,29,334,633);
	//设置右上角小方框
	rectangle(370,50,515,195);

	//设置右下角文字内容属性，分数，等级等
	setfont(24,0,_T("宋体"));
	setcolor(BLUE);
	outtextxy(390,215,_T("下一个方块"));

	setcolor(RED);
	outtextxy(405,280,_T("分数"));
	
	//按指定格式把字符串打印到字符数组里
	sprintf(str,"%d",score);//将分数打印到字符数组str里
	outtextxy(415,310,str);

	outtextxy(405,375,_T("等级"));
	sprintf(str,"%d",rank);
	outtextxy(425,405,str);

	//操作说明
	setcolor(LIGHTBLUE);
	outtextxy(390,475,_T("操作说明"));
	outtextxy(390,500,"↑:旋转");
	outtextxy(390,525,"↓:快速下降");
	outtextxy(390,550,"←:左移");
	outtextxy(390,575,"→:右移");
	outtextxy(390,600,"空格:暂停");

}

//设置小方块■
void clearBlock(void){
	setcolor(BLACK);
	setfont(23,0,"宋体");//将■的大小调大一点

	for(int i = 0;i<BLOCK_HEIGHT;i++){  //第几行
		for(int j = 0;j<BLOCK_WIDTH;j++){  //第几列
			int x = 391 + j * UNIT_SIZE;  //起始坐标，表示方块
			int y = 71 + i * UNIT_SIZE;
			outtextxy(x,y,"■");//"■"，指定位置打印
		}
	}
}

//在右上角小方框内，画下一个方块
void drawBlock(int x,int y){
	setcolor(color[NextIndex]);//调用颜色数组设置方块颜色
	setfont(23,0,"宋体");//将■的大小调大一点

	for(int i = 0;i<BLOCK_HEIGHT;i++){  //第几行
		for(int j = 0;j<BLOCK_WIDTH;j++){  //第几列
			if(block[NextIndex*4][i][j] == 1){		//判断是否为1，为1就画
			int x1 = x + j * UNIT_SIZE;  //起始坐标，表示方块
			int y1 = y + i * UNIT_SIZE;
			outtextxy(x1,y1,"■");//"■"，指定位置打印
			}
		}
	}
}

//指定位置绘制指定方块的指定方向
void drawBlock(int x,int y,int blockIndex,block_dir_t dir){
	setcolor(color[blockIndex]);//调用颜色数组设置方块颜色
	setfont(23,0,"宋体");//将■的大小调大一点
	int id = blockIndex*4+dir;
	for(int i = 0;i<BLOCK_HEIGHT;i++){  //第几行
		for(int j = 0;j<BLOCK_WIDTH;j++){  //第几列
			if(block[id][i][j] == 1){		//判断是否为1，为1就画
			int x1 = x + j * UNIT_SIZE;  //起始坐标，表示方块
			int y1 = y + i * UNIT_SIZE;
			outtextxy(x1,y1,"■");//"■"，指定位置打印
			}
		}
	}
}

//清除指定位置指定方向的方块
//参数x：方块的左上角的x坐标
//参数y:方块左上角在游戏区内的坐标距离游戏区顶部距离
void drawBlock(int x,int y,block_dir_t dir){
	setcolor(BLACK);//擦掉方块（黑色背景）
	int id = BlockIndex * 4 + dir;
	y+=START_Y;
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			if(block[id][i][j] == 1){
				//擦除该方块的第i行的第j列
				outtextxy(x+j*20,y+i*20,"■");
			}
		}
	}
}

void clearBlock(int x,int y,block_dir_t dir){
	setcolor(BLACK);//擦掉方块（黑色背景）
	int id = BlockIndex * 4 + dir;
	y+=START_Y;
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			if(block[id][i][j] == 1){
				//擦除该方块的第i行的第j列
				outtextxy(x+20*j,y+i*20,"■");
			}
		}
	}
}

//产生新方块
void nextblock(void){
	//清除小方框内的内容
	clearBlock();

	//随机选择一种方块（用随机数表示）
	srand(time(NULL));//使用时间函数返回值作为随机种子
	NextIndex = rand() % BLOCK_COUNT;//对五取余，将随机数限制范围

	//定义一个画当前这个方块的函数,在小方框内画
	drawBlock(391,71);
}

//如果在指定位置可以向指定方向移动则返回1，否则为0
int moveable(int x1,int y1,move_dir_t moveDir,block_dir_t blockDir){
	//计算当前方块的左上角在30*15的游戏区中的位置(第几行,第几列)
	int x = (y1 - minY) / UNIT_SIZE;
	int y = (x1 - minX) / UNIT_SIZE;
	int id = BlockIndex * 4 + blockDir; //计算第几个(种)方块
	int ret = 1;//1能运动，0则不能

	if(moveDir == MOVE_DOWN){
		for(int i = 0; i<5;i++){
			for(int j = 0;j<5;j++){
				if(block[id][i][j] == 1 && 
					(x + i + 1>= 30 || visit[x+i+1][y+j]==1)){
						ret = 0;
				}
			}
		}//用数组判断是否空心或实心
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

//检测游戏是否结束
void failCheck(){
	if(!moveable(START_X,START_Y,MOVE_DOWN,BLOCK_UP)){
		setcolor(WHITE);
		setfont(45,0,"宋体");//设置字体
		outtextxy(75,300,"哦豁!玩完了!");//输出
		Sleep(1000);  //休眠1秒
		system("pause");
		closegraph();  //关闭图像界面
		exit(0); //整个程序结束
	}
}

//休眠功能
void wait(int interval){
	int count = interval / 10;//休眠次数
	for(int i=0;i<count;i++){
		Sleep(10);
		if(kbhit()){
			return;
		}
	}
}

//判断当前方块是否可以转向到指定方向
//此时只是判断还没旋转
int rotatable(int x,int y,block_dir_t dir){
	int id = BlockIndex * 4 + dir;
	int xIndex = (y-minY) / 20;//**错误3**
	int yIndex = (x-minX) / 20;

	if(!moveable(x,y,MOVE_DOWN,dir)){
		return 0;
	}//判断方块能否向下滚动
 
	for(int i=0;i<5;i++){
		for(int j = 0;j<5;j++){
			if(block[id][i][j] == 1 &&
				(yIndex+j<0 || yIndex+j>=15 || visit[xIndex+i][yIndex+j]==1)){
				return 0;
			}//判断边界确定旋转
		}
	}
	return 1;	
}

//方块到达边界则固定 
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
	block_dir_t blockDir = BLOCK_UP; //方块的朝向
	int curSpeed = speed;

	//检测游戏是否结束
	failCheck();

	//持续向下落
	while(1){
		
		//检查此时此刻有没有按键按下去,按下去返回1
		if(kbhit()){
			int key = getch();  //整数
			if(key == KEY_SPACE){   //if里面判断用“==”**错误1**
				getch();
			}
		}

		//清除当前方块
		clearBlock(x,k,blockDir);   //blockDir方块朝向

		//判断按键有没有按下去
		if(kbhit()){
			int key = getch();
			
			if(key == KEY_UP){    //向上旋转
				block_dir_t nextDir = (block_dir_t)((blockDir + 1) % 4);//下一个方向
				if(rotatable(x,y+k,nextDir)){
					blockDir = nextDir;
				}
			}else if(key == KEY_DOWN){
				curSpeed = 50;
			}else if(key == KEY_LEFT){
				if(moveable(x,y+k+20,MOVE_LEFT,blockDir)){
					x -= 20;			//一直减无限制**错误4**
				}
			}else if(key == KEY_RIGHT){
				if(moveable(x,y+k+20,MOVE_RIGHT,blockDir)){
					x += 20;          //同理错误4 **错误5**
				}
			}
		}

		k += 20;

		//根据方向键的指令,绘制当前方块
		drawBlock(x,y+k,BlockIndex,blockDir);

		wait(curSpeed);//等待一个时间，可变，Sleep(curSleep)也可以用（有不同，但卡）
		
		//k += 20;此位置不对   **错误2**

		//方块的固化处理,"定格"
		if(!moveable(x,y+k,MOVE_DOWN,blockDir)){
			mark(x,y+k,BlockIndex,blockDir);
			break;
		}
	}
}

//产生新方块
void newblock(void){
	//确定即将使用的方块的类别
	BlockIndex = NextIndex;

	//绘制刚从顶部下降的方块
	drawBlock(START_X,START_Y);//绘制顶部降落方块

	//让新出现的方块暂停一会
	Sleep(100);//0.1秒

	//在右上角区域，绘制下一个方块
	nextblock();

	//方块向下移动（降落）
	move();

}

void down(int x){
	for(int i=x;i>0;i--){
		//消除第i行，第j列的方格消除
		for(int j=0;j<15;j++){
			if(visit[i-1][j]){
				visit[i][j] = 1;
				markColor[i][j] = markColor[i-1][j];
				setcolor(markColor[i][j]);//设置颜色
				outtextxy(20*j+minX,20*i+minY,"■");
			}else{
				visit[i][j] = 0;
				setcolor(BLACK);
				outtextxy(20*j+minX,20*i+minY,"■");
			}
		}
	}
	//清除最顶上的一行（即行标为0的一行）
	setcolor(BLACK);
	for(int j=0;j<15;j++){
		visit[0][j] = 0;
		outtextxy(20*j+minX,minY,"■");
	}
}

//更新分数，lines表示消除的行数
void addScore(int lines){
	char str[32];

	setcolor(RED);
	score += lines * 10;//分数
	sprintf(str,"%d",score);
	outtextxy(415,310,str);
}

//等级更新
void updateGrade(){
	//更新等级的表示,80分1级
	rank = score / 80;
	char str[16];
	sprintf(str,"%d",rank);
	outtextxy(425,405,str);
	//更新速度,等级越高速度越快，睡眠就越短
	//最慢500，最快100（毫秒）
	speed = 500 - rank *100;
	if(speed <= 100){
		speed = 100;
	}
}

//消除满行
void check(void){
	int i,j;
	int clearLines = 0;//每消除一行的分数
	for(i=29;i>=0;i--){
		//检查第i行有没有满
		for(j=0;j<15 && visit[i][j];j++);
			
			//执行到此处时，有两种情况：
			//1.第i行没有满（有空位）此时j<15
			//2.第i行已经满了，此时j>=15
			if(j>=15){
				//此时第i行已满，需消除第i行
				down(i);//消除第i行，并把上面行下移
				i++;   //因为最外层的循环中有i--，所以先i++,使下次循环时，再把这一行检查一下
				clearLines++;
		}
	}
	//更新分数
	addScore(clearLines);

	//更新等级--更新速度
	updateGrade();
}

int main(void){
	welcome();//欢迎界面
	initGameScene();//游戏场景
	
	nextblock();//产生新方块
	Sleep(500);  //暂停500毫秒
	memset(visit,0,sizeof(visit));//初始化访问数组,全设初值为0



	while(1){			//游戏循环
		newblock();

		//消分（消除满行，并更新分数和速度）
		check();
	}
	
	system("pause");//界面暂停
	closegraph();//关闭界面
	return 0;
}