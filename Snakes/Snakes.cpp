#include "pch.h"
#include <iostream>
#include <Windows.h>
#define n 20
#include <conio.h>
#include<time.h>
#include<cstring>
#include<cstdio>
#include "stdlib.h"
using namespace std;
//控制光标
void gotoxy(int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void gotoxy(int x, int y, HANDLE consoleBuf) {
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(consoleBuf, pos);
}
//坐标类
template<typename T>
class sPoint {
public:
	T x, y;
	sPoint() {};
	sPoint(T xval, T yval) :x(xval), y(yval) {};
	sPoint<T> & operator = (const sPoint<T> & rval) {
		this->x = rval.x;
		this->y = rval.y;
		return *this;
	}
};

//方向
enum Direction {
	left='a',right='d'
};

//随机产生一个食物
double * setFood()
{
	double *a=new double[2];
	int x, y;
	srand(time(0));//随机种子
	do {
		x = rand() % 50 + 1;
		y = rand() % 30 + 1;
		break;
	} while (1);
	gotoxy(x, y);
	a[0] = x;
	a[1] = y;//将生成的食物坐标存储起来
	std::cout << '0' ;
	return a;
}
	//生成围墙
	void getWall()
	{
		sPoint<double> wall;
		wall.x = 0;
		wall.y =0;
		while (wall.y <= 30)
		{
			gotoxy(50,wall.y);
			std::cout << "|";
			wall.y++;
		}
		gotoxy(0, 30);
		while (wall.x <= 50)
		{
			std::cout << "-";
			wall.x++;
		}
	}

//蛇类
class Snake {
private:
	sPoint<double> snakeHead;//蛇头的位置
	sPoint<double> snakeTail;//蛇尾的位置
	sPoint<double> snakeBody[n];//蛇身各点
	//int forwardTime;//前进的时间间隔
	Direction direction;
	char sym;
	int last;
public:
	Snake() {};
	Snake(sPoint<double>  head, sPoint<double> tail,int Last,sPoint<double> body[n]) {
		this->snakeHead = head;
		this->snakeTail = tail;
		this->last = Last;
		this->snakeBody[20] = body[20];
		snakeBody[0].x = snakeHead.x;
		snakeBody[0].y = snakeHead.y;
		snakeBody[0].x = 15; snakeBody[0].y = 15; snakeBody[1].x = 16; snakeBody[1].y = 15;
		snakeBody[2].x = 17; snakeBody[2].y = 15; snakeBody[3].x = 18; snakeBody[3].y = 15;
		last = 4;
	}
	//蛇向左转
	void setSnakeLeft()
	{
		//蛇的后一点往前一点移动
		for (int i = last-1; i >= 1; i--)
			snakeBody[i] = snakeBody[i - 1];
		snakeHead.x = snakeHead.x - 1;
		snakeHead.y = snakeHead.y;
		snakeBody[0].x = snakeHead.x;
		snakeBody[0].y = snakeHead.y;
	}
	//蛇向右转
	void setSnakeRight()
	{
		//蛇的后一点往前一点移动
		for (int i = last-1; i >= 1; i--)
			snakeBody[i] = snakeBody[i - 1];
		snakeHead.y = snakeHead.y;
		snakeHead.x = snakeHead.x+1;
		snakeBody[0].x = snakeHead.x;
		snakeBody[0].y = snakeHead.y;
	}
	//蛇向上转
	void setSnakeUp()
	{
		for (int i = last-1; i >= 1; i--)
			snakeBody[i] = snakeBody[i - 1];
		snakeHead.y = snakeHead.y - 1;
		snakeHead.x = snakeHead.x;
		snakeBody[0].x = snakeHead.x;
		snakeBody[0].y = snakeHead.y;
	}
	//蛇向下转
	void setSnakeDown()
	{
		for (int i = last-1; i >= 1; i--)
			snakeBody[i] = snakeBody[i - 1];
		snakeHead.y = snakeHead.y + 1;
		snakeHead.x = snakeHead.x;
		snakeBody[0].x = snakeHead.x;
		snakeBody[0].y = snakeHead.y;
	}
	//蛇直走
	void setSnakeForward()
	{
		if (snakeBody[0].y < snakeBody[1].y)
			snakeHead.y = snakeHead.y - 1;
		if (snakeBody[0].y > snakeBody[1].y)
			snakeHead.y = snakeHead.y + 1;
		if (snakeBody[0].x < snakeBody[1].x)
			snakeHead.x = snakeHead.x - 1;
		if (snakeBody[0].x > snakeBody[1].x)
			snakeHead.x = snakeHead.x + 1;
		for (int i = last; i >= 1; i--)
			snakeBody[i] = snakeBody[i - 1];
		snakeBody[0].x = snakeHead.x;
		snakeBody[0].y = snakeHead.y;
	}
	//蛇身加一点
	void addSnake()
	{
		if (snakeBody[last - 1].y < snakeBody[last - 2].y)
		{
			snakeBody[last] = snakeBody[last - 1];
			snakeBody[last].y--;
		}
		if (snakeBody[last -1].y > snakeBody[last -2].y)
		{
			snakeBody[last] = snakeBody[last - 1];
			snakeBody[last].y++;
		}
		if (snakeBody[last -1].x < snakeBody[last -2].x)
		{
			snakeBody[last] = snakeBody[last - 1];
			snakeBody[last].x--;
		}
		if (snakeBody[last -1].x > snakeBody[last -2].x)
		{
			snakeBody[last] = snakeBody[last - 1];
			snakeBody[last].x++;
		}
		last++;
		snakeHead.x = snakeBody[0].x;
		snakeHead.y = snakeBody[0].y;
	}
    //判断蛇是否撞墙或撞自己
	int ifOut()
	{
		//蛇是否到达边界
		if (snakeBody[0].x == 50 || snakeBody[0].y == 30 || snakeBody[0].x == 0 || snakeBody[0].y == 0)
			return 0;
		//判断蛇是否撞到自己
		for (int j = 1; j <last; j++)
		{
			if ((snakeHead.x == snakeBody[j].x)&&(snakeHead.y == snakeBody[j].y))
				return 0;
		}
		return 1;
	}
	//初步显示蛇的图案
	void snakeDisplay()
	{
		gotoxy(snakeBody[0].x, snakeBody[0].y);
		std::cout << "*";//显示蛇头
		for (int a = 1; a < last; a++)
		{
			//std::cout << this->snakeBody[a].x << this->snakeBody[a].y;
			gotoxy(snakeBody[a].x, snakeBody[a].y);
			std::cout << "#";
		}
	}
	//判断蛇是否吃到食物
	int ifEat(double x,double y)
	{
		if (this->snakeHead.x == x&&this->snakeHead.y == y)
			return true;
		else return false;
	}
};
	int main()	{
		double x, y, *a;
		sPoint<double> body[20];//用于存储身体各点坐标
		system("cls");
		getWall();
		Snake snake = Snake(sPoint<double>(15, 15), sPoint<double>(18, 15), 4, body);//初始化蛇
		snake.snakeDisplay();
		a=setFood();
		x = a[0]; y = a[1];
		char ch = '/0';
		Sleep(5000);
		//当没有键按下，蛇往前走
		while (!_kbhit())
		{
			snake.setSnakeForward();
			Sleep(300);
			system("cls");
			getWall();
			gotoxy(x,y);
			cout << "0";
			snake.snakeDisplay();
			if (snake.ifEat(x, y))
			{
				snake.addSnake();
				system("cls");
				getWall();
				gotoxy(x, y);
				cout << "0";
				setFood();
				snake.snakeDisplay();
			}
			if (!snake.ifOut())
				return 0;
		}
		while (1) {
			if (_kbhit())
			{
				ch = _getche();//ch记录按下的键是什么
				switch (ch)
				{
				case 'd':
					snake.setSnakeRight();
					Sleep(200);
					system("cls");
					getWall();
					gotoxy(x, y);
					cout << "0";
					snake.snakeDisplay();
					break;
				case 'a':
					snake.setSnakeLeft();
					Sleep(200);
					system("cls");
					getWall();
					gotoxy(x, y);
					cout << "0";
					snake.snakeDisplay();
					break;
				case 'w':
					snake.setSnakeUp();
					Sleep(200);
					system("cls");
					getWall();
					gotoxy(x, y);
					cout << "0";
					snake.snakeDisplay();
					break;
				case 's':
					snake.setSnakeDown();
					Sleep(200);
					system("cls");
					getWall();
					gotoxy(x, y);
					cout << "0";
					snake.snakeDisplay();
					break;
				case 'p':
					return 0;
				default:
					break;
				}
			}
			while (!_kbhit())
			{
				snake.setSnakeForward();
				Sleep(300);
				system("cls");
				getWall();
				gotoxy(x, y);
				cout << "0";
				snake.snakeDisplay();
				if (snake.ifEat(x, y))
				{
					snake.addSnake();
					system("cls");
					getWall();
					gotoxy(x, y);
					cout << "0";
					setFood();
					snake.snakeDisplay();
				}
			}
			if (!snake.ifOut())
				return 0;
			if (snake.ifEat(x,y))
			{
				snake.addSnake();
				system("cls");
				getWall();	
				a=setFood();
				x = a[0]; y = a[1];
				snake.snakeDisplay();
			}
		}
		return 0;
}