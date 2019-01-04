#include "graphics.h"
#include"stdio.h"
#include <windows.h>

void menu()//菜单界面
{
	/*-----界面 菜单-----*/
	initgraph(780, 565);//初始化绘图环境 窗口尺寸 长*高=780*565 
	PIMAGE img = newimage();//保存图像的对象 新建图像 在不使用的时候delimage
	getimage(img, ".//interface2.jpg");//获取图像 图片尺寸 410*600
	putimage(0, 0, img); //在屏幕上绘制指定图像 第一二个参数为输出图像的起始位置（左，上）

	setfillcolor(EGERGB(255, 255, 221));//设置填充颜色 浅黄色
	bar(410, 0, 780, 565);//画一个矩形  矩形位置 x轴为410到780；y轴为0到565

	setcolor(EGERGB(10, 0, 10));//文字颜色 黑色
	setfontbkcolor(EGERGB(255, 255, 221));//文字背景色 浅绿色

	setfont(30, 0, "楷体");//设置字体
	outtextxy(425, 80, "哈夫曼编码译码操作系统");//输出文字 第一二个参数设置文字输出起始位置
	setfont(23, 0, "楷体");
	outtextxy(430, 140, "---输入数字完成相应操作---");
	outtextxy(450, 210, "1.执行静态存储操作");
	outtextxy(450, 280, "2.执行动态存储操作");
	outtextxy(450, 350, "3.退出程序");

	/*改变指定窗口的位置和大小.
	第一个参数指定了窗口的句柄
	第二个参数指定了窗口的左边的新位置，第三个窗口是上部的新位置
	第四个是指定窗口宽度，第五个是指定窗口高度*/
	MoveWindow(getHWnd(), 50, 100, 780, 565, TRUE);
}
void cmd_menu()
{
	printf("*****************************************************************************\n");
	printf("|+|■■■■■■■■■■■   哈夫曼编码译码操作系统  ■■■■■■■■■■■|+|\n");
	printf("|+|■-------------------------------------------------------------------■|+|\n");
	printf("|+|■\t\t\t  ★ -1-执行静态存储操作   \t\t\t■|+|\n");
	printf("|+|■\t\t\t  ★ -2-执行动态存储操作   \t\t\t■|+|\n");
	printf("|+|■\t\t\t  ★ -3-退出程序   \t\t\t\t■|+|\n");
	printf("|+|■-------------------------------------------------------------------■|+|\n");
	printf("|+|■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■||+|\n");
	printf("*****************************************************************************\n");
}
void music()//背景音乐
{
	/*-----背景音乐-----*/
	mciSendString("play .//music1.mp3 repeat",        //要发送的命令字符串 循环播放.mp3音乐文件
		NULL,                                //存放反馈信息的缓冲区,为上个参数指定了字符串变量的大小
		0,                                    //缓冲区的长度 字符变量的长度
		NULL);                                //回调方式，一般为NULL
}
//哈夫曼树结点结构
typedef struct {
	int weight;//结点权重
	char code;
	int parent, left, right;//父结点、左孩子、右孩子在数组中的位置下标
}HTNode, *HuffmanTree;
//动态二维数组，存储哈夫曼编码


typedef char ** HuffmanCode;

char kd = 'A';//用于定义树中名称 

//HT数组中存放的哈夫曼树，end表示HT数组中存放结点的最终位置，s1和s2传递的是HT数组中权重值最小的两个结点在数组中的位置
void Select(HuffmanTree HT, int end, int *s1, int *s2)
{
	int min1, min2;
	//遍历数组初始下标为 1
	int i = 1;
	//找到还没构建树的结点
	while (HT[i].parent != 0 && i <= end) {
		i++;
	}
	min1 = HT[i].weight;
	*s1 = i;
	i++;
	while (HT[i].parent != 0 && i <= end) {
		i++;
	}
	//对找到的两个结点比较大小，min2为大的，min1为小的
	if (HT[i].weight < min1) {
		min2 = min1;
		*s2 = *s1;
		min1 = HT[i].weight;
		*s1 = i;
	}
	else {
		min2 = HT[i].weight;
		*s2 = i;
	}
	//两个结点和后续的所有未构建成树的结点做比较
	for (int j = i + 1; j <= end; j++)
	{
		//如果有父结点，直接跳过，进行下一个
		if (HT[j].parent != 0) {
			continue;//跳出本次循环，执行下一次循环
		}
		//如果比最小的还小，将min2=min1，min1赋值新的结点的下标
		if (HT[j].weight < min1) {
			min2 = min1;
			min1 = HT[j].weight;
			*s2 = *s1;
			*s1 = j;
		}
		//如果介于两者之间，min2赋值为新的结点的位置下标
		else if (HT[j].weight >= min1 && HT[j].weight < min2) {
			min2 = HT[j].weight;
			*s2 = j;
		}
	}

}

//HT为地址传递的存储哈夫曼树的数组，w为存储结点权重值的数组，n为结点个数
void CreateHuffmanTree(HuffmanTree *HT, int *w, int n)
{
	if (n <= 1) return; // 如果只有一个编码就相当于0
	int m = 2 * n - 1; // 哈夫曼树总节点数，n就是叶子结点
	*HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode)); // 0号位置不用
	HuffmanTree p = *HT;
	// 初始化哈夫曼树中的所有结点
	for (int i = 1; i <= n; i++)
	{
		(p + i)->code = 0;
		(p + i)->weight = *(w + i - 1);
		(p + i)->parent = 0;
		(p + i)->left = 0;
		(p + i)->right = 0;
	}
	//从树组的下标 n+1 开始初始化哈夫曼树中除叶子结点外的结点
	for (int i = n + 1; i <= m; i++)
	{
		(p + i)->code = 0;
		(p + i)->weight = 0;
		(p + i)->parent = 0;
		(p + i)->left = 0;
		(p + i)->right = 0;
	}
	//构建哈夫曼树
	char cd = 'A';
	for (int i = n + 1; i <= m; i++)//i用来存储非叶子结点的地址
	{
		int count = 0;
		int s1, s2;
		Select(*HT, i - 1, &s1, &s2);

		if (count == 0) {
			(*HT)[s1].code = ' ';
			(*HT)[s2].code = cd;
			count++;
		}
		else {
			cd = cd + 1;
			(*HT)[s1].code = cd;
			cd = cd + 1;
			(*HT)[s2].code = cd;
			count++;
		}
		(*HT)[s1].parent = (*HT)[s2].parent = i;
		(*HT)[i].left = s1;
		(*HT)[i].right = s2;
		(*HT)[i].weight = (*HT)[s1].weight + (*HT)[s2].weight;


	}
}

//打印哈夫曼树
void  PrintHuffmanTree(HuffmanTree *HT, int *w, int n)
{
	int i;
	int m = 2 * n - 1; // 哈夫曼树总节点数，n就是叶子结点
	printf("\n打印该哈夫曼树：\n");
	printf(" 权值    双亲   左孩子  右孩子\n");
	for (i = 1; i <= m; i++)
	{
		printf("%5d %7d %7d %7d\n", (*HT)[i].weight, (*HT)[i].parent, (*HT)[i].left, (*HT)[i].right);
	}
}

//将哈夫曼树写入文件
void WriteTree(HuffmanTree *HT, int *w, int n)
{
	FILE *fp;
	int m = 2 * n - 1;
	int i;
	if ((fp = fopen(".//data4.txt", "w")) == NULL)//打开文件 
	{
		printf("cannot open data.txt!");
		exit(0);
	}
	else
	{
		printf("............................................\n");
		printf("成功打开data4.txt文件！\n");
		printf("............................................\n");
	}
	//将哈夫曼树写入文件 
	for (i = 1; i <= m; i++)
	{
		fprintf(fp, "%d %d %d %d\n", (*HT)[i].weight, (*HT)[i].parent, (*HT)[i].left, (*HT)[i].right);
	}
	//关闭文件
	if (fclose(fp))
	{
		printf(" 文件关闭失败！！!\n");
		exit(0);
	}
}

//HT为哈夫曼树，HC为存储结点哈夫曼编码的二维动态数组，n为结点的个数
void HuffmanCoding(HuffmanTree HT, HuffmanCode *HC, int n) {
	*HC = (HuffmanCode)malloc((n + 1) * sizeof(char *));
	char *cd = (char *)malloc(n * sizeof(char)); //存放结点哈夫曼编码的字符串数组
	cd[n - 1] = '\0';//字符串结束符

	for (int i = 1; i <= n; i++) {
		//从叶子结点出发，得到的哈夫曼编码是逆序的，需要在字符串数组中逆序存放
		int start = n - 1;
		//当前结点在数组中的位置
		int c = i;
		//当前结点的父结点在数组中的位置
		int j = HT[i].parent;
		// 一直寻找到根结点
		while (j != 0)
		{
			// 如果该结点是父结点的左孩子则对应路径编码为0，否则为右孩子编码为1
			if (HT[j].left == c)
				cd[--start] = '0';
			else
				cd[--start] = '1';
			//以父结点为孩子结点，继续朝树根的方向遍历
			c = j;
			j = HT[j].parent;
		}
		//跳出循环后，cd数组中从下标 start 开始，存放的就是该结点的哈夫曼编码
		(*HC)[i] = (char *)malloc((n - start) * sizeof(char));
		strcpy((*HC)[i], &cd[start]);
	}
	//使用malloc申请的cd动态数组需要手动释放
	free(cd);
}

//打印哈夫曼编码的函数
void PrintHuffmanCode(HuffmanCode htable, int *w, int n)
{
	char code = ' ';//初始化code为空格字符
	printf("\nHuffman code : \n");
	for (int i = 1; i <= n; i++) {
		if (code == 'A')
		{
			printf("%c", code);
			printf("=");
			printf("%s\n", htable[i]);
			code = code + 1;
		}
		else {
			printf("%c", code);
			printf("=");
			printf("%s\n", htable[i]);
			code = code + 1;
		}
		if (i == 1)
		{
			code = 'A';
		}
	}
}
//哈夫曼编码写入文件
void WriteHuffmanCode(HuffmanCode htable, int *w, int n)
{
	FILE *fp;
	char code = ' ';
	int i;
	if ((fp = fopen(".//data3.txt", "w")) == NULL)//打开文件 
	{
		printf("cannot open data.txt!");
		exit(0);
	}
	else
	{
		printf(".............................................\n");
		printf("成功打开data3.txt文件！\n");
	}
	//打开文件后进行写入文件操作 

	for (i = 1; i <= n; i++)
	{

		if (code == 'A')
		{
			//			fprintf(fp,"%c",code);
			//    		fprintf(fp,"=");
			fprintf(fp, "%s\n", htable[i]);
			code = code + 1;
		}
		else
		{
			//			fprintf(fp,"%c",code);
			//    		fprintf(fp,"=");
			fprintf(fp, "%s\n", htable[i]);
			code = code + 1;
		}
		if (i == 1) {
			code = 'A';
		}
	}

	printf("\n哈夫曼编码成功写入文件！！\n");
	if (fclose(fp))
	{
		printf(" 文件关闭失败！！!\n");
		exit(0);
	}
}
//哈夫曼存入文件 
void WriteHuffmanCode2(HuffmanCode htable, int *w, int n)
{
	FILE *fp;
	char code = ' ';
	int i;
	if ((fp = fopen(".//data5.txt", "w")) == NULL)//打开文件 
	{
		printf("cannot open data.txt!");
		exit(0);
	}
	else
	{
		printf(".............................................\n");
		printf("成功打开data5.txt文件！\n");
	}
	//打开文件后进行写入文件操作 

	for (i = 1; i <= n; i++)
	{

		if (code == 'A')
		{
			fprintf(fp, "%c", code);
			fprintf(fp, "=");
			fprintf(fp, "%s\n", htable[i]);
			code = code + 1;
		}
		else
		{
			fprintf(fp, "%c", code);
			fprintf(fp, "=");
			fprintf(fp, "%s\n", htable[i]);
			code = code + 1;
		}
		if (i == 1) {
			code = 'A';
		}
	}

	printf("\n哈夫曼编码成功写入文件！！\n");
	printf(".............................................\n");
	if (fclose(fp))
	{
		printf("文件关闭失败！！!\n");
		exit(0);
	}
}

//哈夫曼译码
void HuffmanDecoding(HuffmanTree HT, int n, int *w)
{
	char s[100];
	int num;//存放输入的哈夫曼编码
	int a = 0;

	char ch = 'A';
	FILE *fp;
	int i = 0, g = 2 * n - 1;//g在根节点位置;
	printf("\n请输入哈夫曼编码进行哈夫曼译码\n:");
	scanf("%d", &num);
	printf("则译码结果为:");
	while (s[i] != '\0')//判断哈夫曼编码段中元素
	{
		if (s[i] == '0')//划至左子树查找 
			g = HT[g].left;
		if (s[i] == '1')//右子树查找 
			g = HT[g].right;
		if (!HT[g].left && !HT[g].right)//如果左右子树都没有结点则为叶子结点，则找到 
		{
			g = 2 * n - 1;//将指针返回哈夫曼树的根节点 
		}
		i++;//未找见继续循环 
	}
	if ((fp = fopen(".//data3.txt", "r")) == NULL)
	{
		printf("cannot open data3.txt");
		exit(0);
	}
	else//
		{
		for (a = 0; a < n; a++) {
			fscanf(fp, "%d ", &w[a]);
		}
	}
	fclose(fp);
	for (a = 0; a < n; a++) {
		if (num == w[a] && a == 0) {
			printf(" ");
			break;
		}
		else if (num == w[a] && a != 0) {
			ch = ch - 1;
			printf("%c", ch);
			break;
		}
		else {
			ch = ch + 1;
		}
	}
	printf("\n译码结束");
}


//静态存储权值 
int sca1()
{
	//将权值存入data.txt 
	int w[27] = { 186 ,64, 13 ,22 ,32 ,103, 21, 15 ,47, 57, 1 ,5, 32, 20,57 ,63 ,15 ,1 ,48 ,51 ,80 ,23, 8 ,18 ,1 ,16 ,1 };
	FILE *fp;
	int n = 27;
	if ((fp = fopen(".//data.txt", "w")) == NULL)
	{
		printf("cannot open data.txt!");
		exit(0);
	}
	else
	{
		for (int i = 0; i < n; i++)//格式化输出到一个流文件中
		{
			fprintf(fp, "%d\n", w[i]);
		}
	}
	fclose(fp);
	return n;
}
//读取文件中的权值
void rea1(int n1, int *w) {
	//从文件读取权值进行编码 
	FILE *fp1;
	if ((fp1 = fopen(".//data.txt", "r")) == NULL) {
		printf("cannot open data.txt!");
		exit(0);
	}
	else {
		for (int i = 0; i < n1; i++) {
			fscanf(fp1, "%3d", &w[i]);
		}
	}
	fclose(fp1);
}
//附加功能1：从键盘输入权值数据存入文件中，构建哈夫曼树进行相应操作（动态） 
int sca2(HuffmanTree HT)
{
	int n;
	int a[30];
	printf("请输入要构建的权值个数：\n");
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
	{
		printf("\n请输入权值：");
		scanf("%d", &a[i]);
	}
	FILE *fp;
	if ((fp = fopen(".\\data2.txt", "w")) == NULL)
	{
		printf("cannot open data2.txt");
		exit(0);
	}
	else {
		for (int j = 0; j < n; j++) {
			fprintf(fp, "%d\n", a[j]);
		}
	}
	fclose(fp);
	return n;
}
//读取data2.txt 
void rea2(int n1, int *w) {
	//从文件读取权值进行编码 
	FILE *fp1;
	if ((fp1 = fopen(".\\data2.txt", "r")) == NULL) {
		printf("cannot open data2.txt!");
		exit(0);
	}
	else {
		for (int i = 0; i < n1; i++) {
			fscanf(fp1, "%3d", &w[i]);
		}
	}
	fclose(fp1);
}




int num;
int w[30];
HuffmanTree htree = NULL;
HuffmanCode htable;

void use()
{
	CreateHuffmanTree(&htree, w, num);//创建哈夫曼树 
	PrintHuffmanTree(&htree, w, num);//打印哈夫曼树
	WriteTree(&htree, w, num);//将哈夫曼树写入文件 data4.txt
	HuffmanCoding(htree, &htable, num);//哈夫曼编码 
	PrintHuffmanCode(htable, w, num);//打印哈夫曼编码的函数
	WriteHuffmanCode(htable, w, num);//将哈夫曼编码写入文件 data3.txt
	WriteHuffmanCode2(htable, w, num);//哈夫曼存入文件 data5.txt
	HuffmanDecoding(htree, num, w);//哈夫曼译码
}
int main()
{
	music();//背景音乐
	system("color B0");//第一二个数字分别是背景色和字体色 B是淡浅绿色 0是黑色
	cmd_menu();
	printf("\n-------------------------------按回车键进入程序-----------------------------\n");
	getchar();
	menu();
	while (1) {
		system("color FD");//第一二个数字分别是背景色和字体色 F 亮白色 D 紫色
		char c = getchar();
		mciSendString("pause music1.mp3", NULL, 0, NULL)
		switch (c)
		{
		case '1'://静态存储
			system("color B4");//背景 淡浅绿色；文字 红色
			num = sca1();//将权值存入data.txt 返回num=27
			rea1(num, w); //从 data.txt 文件中读取权值
			use();
			getchar();
			break;
		case '2'://动态存储 
			system("color B4");//背景 淡浅绿色；文字 红色
			num = sca2(htree);//动态存储权值和字符 
			rea2(num, w);//从文件中读取 data2.txt
			use();
			getchar();
			break;
		case '3'://退出程序
			exit(0);
			break;
		default:
			printf("输入错误 请重新输入：\n");
			break;
		}
		getchar();
		system("CLS");//清屏
		cmd_menu();
	}

	return 0;
}
