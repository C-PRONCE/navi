// Ž��Ȱ������2 - �̼�3(�����̼� )

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
 
#define MAX_INT 2147483647 	// �� ������ �Ÿ� �ִ밪 

int n, m;	// n: �����ǰ���, m: ������ ����
char city1[32], city2[32]; 	// ��θ� Ȯ���� ����
int k1, k2; 	// ������ ID
int speed;		// �ڵ��� �ӵ� 

int **graph;	// ���� ��� 
int *visit; 	// �湮 �迭
int *D;		// ��� ��  
int *P;	// �� ���� ��
int *pass;	// �̵� ��� ��
char **city;	// ���� �̸� 

// ������� �ʱ�ȭ 
void initGraph();  

// file "map.txt" ���� �����͸� �Է¹���
int readMap();
// file "city.txt" ���� ���� �̸��� �Է¹���  
int readCity();
// ��� �� �Ÿ��� ����� ���� �Է� 
int inputCity();
// ID(v1)���� �� ���ñ����� �Ÿ� ��� 
void dfs(int v1);
// ��� ���ÿ��� ���� ������������ �̵� ��� ���  
void prnNode();


int main(){

	int i;
	
	// file "map.txt" ���� �����͸� �Է¹���	
	readMap();
	// file "city.txt" ���� ���� �̸��� �Է¹���  
	readCity();	
	

	// ��� �� �Ÿ��� ����� ����(console)
	inputCity();

	D[k1] = 0;	// ���� ������ ��ΰ��� 0 
	P[k1] = MAX_INT;	// �������� �� ���ô� ����  
	
	// ID(v1)���� �� ���ñ����� �Ÿ� ���
	printf("\n<< ��� ��>>\n");	
	dfs(k1);
	
	if( D[k2] != MAX_INT )	prnNode();
	else {
		printf("\n========================================.\n");
		printf("�ΰ��� ���ð� ������� �ʾҽ��ϴ�.");
		printf("\n========================================.\n");
	}
	
	// �޸� �Ҵ� ���� 
	for(i=0; i<n; i++) {
		free(graph[i]);	
		free(city[i]);
	}
	free(graph);
	free(city);
	free(visit);
	free(D);
	free(P);
	free(pass);
	
	return 0;
}

// ������� �ʱ�ȭ 
void initGraph(){
	int i, j;
	
	// **(2��������) �ּ� �Ҵ� 
	graph = (int **) malloc( sizeof(int *) * n);
	city = (char **) malloc( sizeof(char *) * n);
	for(i=0; i<n; i++){
		graph[i] = (int *) malloc( sizeof(int) * n);
		city[i] = (char *) malloc( sizeof(char) * 32);
	}
	visit = (int *) malloc(sizeof(int) * n);
	D = (int *) malloc(sizeof(int) * n);
	P = (int *) malloc(sizeof(int) * n);
	pass = (int *) malloc(sizeof(int) * n);

	// �迭 �ʱ�ȭ  
	for(i=0; i<n; i++) {
		for(j=0; j<n; j++)
			graph[i][j] = 0;
		visit[i] = 0;
		D[i] = MAX_INT;	// ��� ��ΰ��� �ִ밪���� ����  
		P[i] = 0;
		pass[i] = 0;
	}
}

// ID(v1)���� �� ���ñ����� �Ÿ� ���  
void dfs(int v1){
	int i, j;

	visit[v1] = 1;
	
	for(i=0; i<n; i++) {
		if( visit[i]==0 && graph[v1][i]!=0 ) {
			if( (D[v1]+graph[v1][i]) < D[i] ) {
				D[i] = D[v1] + graph[v1][i];
				P[i] = v1;	
			}
			dfs(i);
		}
		putchar('.');	// ��� �� ȭ�鿡 '.' ��� 
	}
	
	for(i=0; i<n; i++) {
		for(j=0; j<n; j++) {
			if( graph[i][j]!=0 ) {
				if( (D[i]+graph[i][j]) < D[j] && D[i] != MAX_INT ) {
					D[j] = D[i] + graph[i][j];
					P[j] = i;	
				}
			}
		putchar('.');	// ��� �� ȭ�鿡 '.' ��� 
		}
	}	
	
}

// ��� ���ÿ��� ���� ������������ �̵� ��� ���  
void prnNode(){
	int i, count;
	int hour, min; 
	float time;
	
	count = 0;
	i = k2;
	
	while( i != k1 ) {
		pass[count] = i;
		i = P[i];
		count++;
	}
	pass[count] = i;

	printf("\n\n========================================");
	printf("\n�̵� ���: " );	
	for(i=count; i>0; i--)
		printf("%s -> ", city[pass[i]] );
	printf("%s", city[pass[0]]);
	
	printf("\n�� �̵��Ÿ� : %d", D[k2] );
	time = (float) D[k2] / speed;
	hour = (int) time;
	min = (int) ((time-(int)time) * 60);
	printf("\n�ӵ� %dkm/h �� ���� ���� �ð� : %d�ð� %d��", speed, hour, min);
	printf("\n========================================\n");
		
}

// file "city.txt" ���� ���� �̸��� �Է¹���
int readMap(){
	int i;
	int a, b, c;	// �Է� : ���ù�ȣ1, ���ù�ȣ2, �Ÿ���  
	FILE *file;
	
	// "map.txt"���� �� �б� �������� ���� 
	file = fopen("map.txt", "r");
	
	// ������ ���� ���ϸ� �޽��� ��� �� ���α׷��� �����Ѵ�.		
	if( file == NULL )	{
		printf("File(map.txt) not found.\n");
		exit(1);
	}

	fscanf(file, "%d %d", &n, &m); 	// �Է�: ������ ��, ���� ������ �� 

	// ������� �ʱ�ȭ 
	initGraph();

	for(i=0; i<m; i++){
		fscanf(file, "%d %d %d", &a, &b, &c);	// ���� ��ȣ �Է�, ����ġ �Է�    
		graph[a][b] = graph[b][a] = c; 	// ���� ��Ŀ� ����ġ ǥ��   
	}

	fclose(file);
	 
	return 0;
}

// file "city.txt" ���� ���� �̸��� �Է¹���  
int readCity() {
	int i;
	FILE *file;
	
	// "city.txt"���� �� �б� �������� ���� 
	file = fopen("city.txt", "r");
	
	// ������ ���� ���ϸ� �޽��� ��� �� ���α׷��� �����Ѵ�.
	if( file == NULL )	{
		printf("File(city.txt) not found.\n");
		
		// �޸� �Ҵ� ���� 
		for(i=0; i<n; i++) {
			free(graph[i]);	
			free(city[i]);
		}
		free(graph);
		free(city);
		free(visit);
		free(D);
		free(P);
		free(pass);		
		
		exit(1); 
	}	

	for(i=0; i<n; i++)
		fscanf(file, "%s", city[i]);

	fclose(file);
	
	return 0;
}

// ��� �� �Ÿ��� ����� ���� �Է�
int inputCity(){
	int chk1=0, chk2=0;
	int i;
	
	printf("�Ÿ� �� ��θ� ����� 2���� ���� �̸��� �Է����ּ��� "); 
	printf("\n���� �̸��� �������� ����ּ���."); 

	do {
		chk1 = chk2 = 0;
		printf("\n��ߵ��� ��������: ");
		scanf("%s %s", city1, city2);
		
		for(i=0; i<n; i++) 
			if (strcmp(city[i], city1)==0) {
				k1=i;
				chk1 = 1;
				break;
			}
		for(i=0; i<n; i++)
			if (strcmp(city[i], city2)==0) {
				k2=i;
				chk2 = 1;
				break;
			}
		
		if(chk1==0||chk2==0)
			printf("   => Error! city.txt ���� ���뿡 ���� �����̸��� �Է��ϼ̽��ϴ�. �ٽ� �Է��Ͽ� �ּ���.");
	} while( chk1==0 || chk2==0 );
		
	while(1) {
		printf("\n�̵� �ӵ��� �Է����ּ���.\n");
		printf("1 ~ 200 ������ ������ �Է����ּ��� \n");
		printf("�ӵ�(km/h) : ");
		scanf("%d", &speed); 
		if (speed>1 && speed<=200) break;
	}

	return 0;	
}

