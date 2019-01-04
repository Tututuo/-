#include "graphics.h"
#include"stdio.h"
#include <windows.h>

void menu()//�˵�����
{
	/*-----���� �˵�-----*/
	initgraph(780, 565);//��ʼ����ͼ���� ���ڳߴ� ��*��=780*565 
	PIMAGE img = newimage();//����ͼ��Ķ��� �½�ͼ�� �ڲ�ʹ�õ�ʱ��delimage
	getimage(img, ".//interface2.jpg");//��ȡͼ�� ͼƬ�ߴ� 410*600
	putimage(0, 0, img); //����Ļ�ϻ���ָ��ͼ�� ��һ��������Ϊ���ͼ�����ʼλ�ã����ϣ�

	setfillcolor(EGERGB(255, 255, 221));//���������ɫ ǳ��ɫ
	bar(410, 0, 780, 565);//��һ������  ����λ�� x��Ϊ410��780��y��Ϊ0��565

	setcolor(EGERGB(10, 0, 10));//������ɫ ��ɫ
	setfontbkcolor(EGERGB(255, 255, 221));//���ֱ���ɫ ǳ��ɫ

	setfont(30, 0, "����");//��������
	outtextxy(425, 80, "�����������������ϵͳ");//������� ��һ���������������������ʼλ��
	setfont(23, 0, "����");
	outtextxy(430, 140, "---�������������Ӧ����---");
	outtextxy(450, 210, "1.ִ�о�̬�洢����");
	outtextxy(450, 280, "2.ִ�ж�̬�洢����");
	outtextxy(450, 350, "3.�˳�����");

	/*�ı�ָ�����ڵ�λ�úʹ�С.
	��һ������ָ���˴��ڵľ��
	�ڶ�������ָ���˴��ڵ���ߵ���λ�ã��������������ϲ�����λ��
	���ĸ���ָ�����ڿ�ȣ��������ָ�����ڸ߶�*/
	MoveWindow(getHWnd(), 50, 100, 780, 565, TRUE);
}
void cmd_menu()
{
	printf("*****************************************************************************\n");
	printf("|+|����������������������   �����������������ϵͳ  ����������������������|+|\n");
	printf("|+|��-------------------------------------------------------------------��|+|\n");
	printf("|+|��\t\t\t  �� -1-ִ�о�̬�洢����   \t\t\t��|+|\n");
	printf("|+|��\t\t\t  �� -2-ִ�ж�̬�洢����   \t\t\t��|+|\n");
	printf("|+|��\t\t\t  �� -3-�˳�����   \t\t\t\t��|+|\n");
	printf("|+|��-------------------------------------------------------------------��|+|\n");
	printf("|+|����������������������������������������������������������������������||+|\n");
	printf("*****************************************************************************\n");
}
void music()//��������
{
	/*-----��������-----*/
	mciSendString("play .//music1.mp3 repeat",        //Ҫ���͵������ַ��� ѭ������.mp3�����ļ�
		NULL,                                //��ŷ�����Ϣ�Ļ�����,Ϊ�ϸ�����ָ�����ַ��������Ĵ�С
		0,                                    //�������ĳ��� �ַ������ĳ���
		NULL);                                //�ص���ʽ��һ��ΪNULL
}
//�����������ṹ
typedef struct {
	int weight;//���Ȩ��
	char code;
	int parent, left, right;//����㡢���ӡ��Һ����������е�λ���±�
}HTNode, *HuffmanTree;
//��̬��ά���飬�洢����������


typedef char ** HuffmanCode;

char kd = 'A';//���ڶ����������� 

//HT�����д�ŵĹ���������end��ʾHT�����д�Ž�������λ�ã�s1��s2���ݵ���HT������Ȩ��ֵ��С����������������е�λ��
void Select(HuffmanTree HT, int end, int *s1, int *s2)
{
	int min1, min2;
	//���������ʼ�±�Ϊ 1
	int i = 1;
	//�ҵ���û�������Ľ��
	while (HT[i].parent != 0 && i <= end) {
		i++;
	}
	min1 = HT[i].weight;
	*s1 = i;
	i++;
	while (HT[i].parent != 0 && i <= end) {
		i++;
	}
	//���ҵ����������Ƚϴ�С��min2Ϊ��ģ�min1ΪС��
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
	//�������ͺ���������δ���������Ľ�����Ƚ�
	for (int j = i + 1; j <= end; j++)
	{
		//����и���㣬ֱ��������������һ��
		if (HT[j].parent != 0) {
			continue;//��������ѭ����ִ����һ��ѭ��
		}
		//�������С�Ļ�С����min2=min1��min1��ֵ�µĽ����±�
		if (HT[j].weight < min1) {
			min2 = min1;
			min1 = HT[j].weight;
			*s2 = *s1;
			*s1 = j;
		}
		//�����������֮�䣬min2��ֵΪ�µĽ���λ���±�
		else if (HT[j].weight >= min1 && HT[j].weight < min2) {
			min2 = HT[j].weight;
			*s2 = j;
		}
	}

}

//HTΪ��ַ���ݵĴ洢�������������飬wΪ�洢���Ȩ��ֵ�����飬nΪ������
void CreateHuffmanTree(HuffmanTree *HT, int *w, int n)
{
	if (n <= 1) return; // ���ֻ��һ��������൱��0
	int m = 2 * n - 1; // ���������ܽڵ�����n����Ҷ�ӽ��
	*HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode)); // 0��λ�ò���
	HuffmanTree p = *HT;
	// ��ʼ�����������е����н��
	for (int i = 1; i <= n; i++)
	{
		(p + i)->code = 0;
		(p + i)->weight = *(w + i - 1);
		(p + i)->parent = 0;
		(p + i)->left = 0;
		(p + i)->right = 0;
	}
	//��������±� n+1 ��ʼ��ʼ�����������г�Ҷ�ӽ����Ľ��
	for (int i = n + 1; i <= m; i++)
	{
		(p + i)->code = 0;
		(p + i)->weight = 0;
		(p + i)->parent = 0;
		(p + i)->left = 0;
		(p + i)->right = 0;
	}
	//������������
	char cd = 'A';
	for (int i = n + 1; i <= m; i++)//i�����洢��Ҷ�ӽ��ĵ�ַ
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

//��ӡ��������
void  PrintHuffmanTree(HuffmanTree *HT, int *w, int n)
{
	int i;
	int m = 2 * n - 1; // ���������ܽڵ�����n����Ҷ�ӽ��
	printf("\n��ӡ�ù���������\n");
	printf(" Ȩֵ    ˫��   ����  �Һ���\n");
	for (i = 1; i <= m; i++)
	{
		printf("%5d %7d %7d %7d\n", (*HT)[i].weight, (*HT)[i].parent, (*HT)[i].left, (*HT)[i].right);
	}
}

//����������д���ļ�
void WriteTree(HuffmanTree *HT, int *w, int n)
{
	FILE *fp;
	int m = 2 * n - 1;
	int i;
	if ((fp = fopen(".//data4.txt", "w")) == NULL)//���ļ� 
	{
		printf("cannot open data.txt!");
		exit(0);
	}
	else
	{
		printf("............................................\n");
		printf("�ɹ���data4.txt�ļ���\n");
		printf("............................................\n");
	}
	//����������д���ļ� 
	for (i = 1; i <= m; i++)
	{
		fprintf(fp, "%d %d %d %d\n", (*HT)[i].weight, (*HT)[i].parent, (*HT)[i].left, (*HT)[i].right);
	}
	//�ر��ļ�
	if (fclose(fp))
	{
		printf(" �ļ��ر�ʧ�ܣ���!\n");
		exit(0);
	}
}

//HTΪ����������HCΪ�洢������������Ķ�ά��̬���飬nΪ���ĸ���
void HuffmanCoding(HuffmanTree HT, HuffmanCode *HC, int n) {
	*HC = (HuffmanCode)malloc((n + 1) * sizeof(char *));
	char *cd = (char *)malloc(n * sizeof(char)); //��Ž�������������ַ�������
	cd[n - 1] = '\0';//�ַ���������

	for (int i = 1; i <= n; i++) {
		//��Ҷ�ӽ��������õ��Ĺ���������������ģ���Ҫ���ַ���������������
		int start = n - 1;
		//��ǰ����������е�λ��
		int c = i;
		//��ǰ���ĸ�����������е�λ��
		int j = HT[i].parent;
		// һֱѰ�ҵ������
		while (j != 0)
		{
			// ����ý���Ǹ������������Ӧ·������Ϊ0������Ϊ�Һ��ӱ���Ϊ1
			if (HT[j].left == c)
				cd[--start] = '0';
			else
				cd[--start] = '1';
			//�Ը����Ϊ���ӽ�㣬�����������ķ������
			c = j;
			j = HT[j].parent;
		}
		//����ѭ����cd�����д��±� start ��ʼ����ŵľ��Ǹý��Ĺ���������
		(*HC)[i] = (char *)malloc((n - start) * sizeof(char));
		strcpy((*HC)[i], &cd[start]);
	}
	//ʹ��malloc�����cd��̬������Ҫ�ֶ��ͷ�
	free(cd);
}

//��ӡ����������ĺ���
void PrintHuffmanCode(HuffmanCode htable, int *w, int n)
{
	char code = ' ';//��ʼ��codeΪ�ո��ַ�
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
//����������д���ļ�
void WriteHuffmanCode(HuffmanCode htable, int *w, int n)
{
	FILE *fp;
	char code = ' ';
	int i;
	if ((fp = fopen(".//data3.txt", "w")) == NULL)//���ļ� 
	{
		printf("cannot open data.txt!");
		exit(0);
	}
	else
	{
		printf(".............................................\n");
		printf("�ɹ���data3.txt�ļ���\n");
	}
	//���ļ������д���ļ����� 

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

	printf("\n����������ɹ�д���ļ�����\n");
	if (fclose(fp))
	{
		printf(" �ļ��ر�ʧ�ܣ���!\n");
		exit(0);
	}
}
//�����������ļ� 
void WriteHuffmanCode2(HuffmanCode htable, int *w, int n)
{
	FILE *fp;
	char code = ' ';
	int i;
	if ((fp = fopen(".//data5.txt", "w")) == NULL)//���ļ� 
	{
		printf("cannot open data.txt!");
		exit(0);
	}
	else
	{
		printf(".............................................\n");
		printf("�ɹ���data5.txt�ļ���\n");
	}
	//���ļ������д���ļ����� 

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

	printf("\n����������ɹ�д���ļ�����\n");
	printf(".............................................\n");
	if (fclose(fp))
	{
		printf("�ļ��ر�ʧ�ܣ���!\n");
		exit(0);
	}
}

//����������
void HuffmanDecoding(HuffmanTree HT, int n, int *w)
{
	char s[100];
	int num;//�������Ĺ���������
	int a = 0;

	char ch = 'A';
	FILE *fp;
	int i = 0, g = 2 * n - 1;//g�ڸ��ڵ�λ��;
	printf("\n�����������������й���������\n:");
	scanf("%d", &num);
	printf("��������Ϊ:");
	while (s[i] != '\0')//�жϹ������������Ԫ��
	{
		if (s[i] == '0')//�������������� 
			g = HT[g].left;
		if (s[i] == '1')//���������� 
			g = HT[g].right;
		if (!HT[g].left && !HT[g].right)//�������������û�н����ΪҶ�ӽ�㣬���ҵ� 
		{
			g = 2 * n - 1;//��ָ�뷵�ع��������ĸ��ڵ� 
		}
		i++;//δ�Ҽ�����ѭ�� 
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
	printf("\n�������");
}


//��̬�洢Ȩֵ 
int sca1()
{
	//��Ȩֵ����data.txt 
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
		for (int i = 0; i < n; i++)//��ʽ�������һ�����ļ���
		{
			fprintf(fp, "%d\n", w[i]);
		}
	}
	fclose(fp);
	return n;
}
//��ȡ�ļ��е�Ȩֵ
void rea1(int n1, int *w) {
	//���ļ���ȡȨֵ���б��� 
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
//���ӹ���1���Ӽ�������Ȩֵ���ݴ����ļ��У�������������������Ӧ��������̬�� 
int sca2(HuffmanTree HT)
{
	int n;
	int a[30];
	printf("������Ҫ������Ȩֵ������\n");
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
	{
		printf("\n������Ȩֵ��");
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
//��ȡdata2.txt 
void rea2(int n1, int *w) {
	//���ļ���ȡȨֵ���б��� 
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
	CreateHuffmanTree(&htree, w, num);//������������ 
	PrintHuffmanTree(&htree, w, num);//��ӡ��������
	WriteTree(&htree, w, num);//����������д���ļ� data4.txt
	HuffmanCoding(htree, &htable, num);//���������� 
	PrintHuffmanCode(htable, w, num);//��ӡ����������ĺ���
	WriteHuffmanCode(htable, w, num);//������������д���ļ� data3.txt
	WriteHuffmanCode2(htable, w, num);//�����������ļ� data5.txt
	HuffmanDecoding(htree, num, w);//����������
}
int main()
{
	music();//��������
	system("color B0");//��һ�������ֱַ��Ǳ���ɫ������ɫ B�ǵ�ǳ��ɫ 0�Ǻ�ɫ
	cmd_menu();
	printf("\n-------------------------------���س����������-----------------------------\n");
	getchar();
	menu();
	while (1) {
		system("color FD");//��һ�������ֱַ��Ǳ���ɫ������ɫ F ����ɫ D ��ɫ
		char c = getchar();
		mciSendString("pause music1.mp3", NULL, 0, NULL)
		switch (c)
		{
		case '1'://��̬�洢
			system("color B4");//���� ��ǳ��ɫ������ ��ɫ
			num = sca1();//��Ȩֵ����data.txt ����num=27
			rea1(num, w); //�� data.txt �ļ��ж�ȡȨֵ
			use();
			getchar();
			break;
		case '2'://��̬�洢 
			system("color B4");//���� ��ǳ��ɫ������ ��ɫ
			num = sca2(htree);//��̬�洢Ȩֵ���ַ� 
			rea2(num, w);//���ļ��ж�ȡ data2.txt
			use();
			getchar();
			break;
		case '3'://�˳�����
			exit(0);
			break;
		default:
			printf("������� ���������룺\n");
			break;
		}
		getchar();
		system("CLS");//����
		cmd_menu();
	}

	return 0;
}
