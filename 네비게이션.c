// 탐구활동보고서2 - 미션3(도전미션 )

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
 
#define MAX_INT 2147483647 	// 각 정점의 거리 최대값 

int n, m;	// n: 정점의개수, m: 간선의 개수
char city1[32], city2[32]; 	// 경로를 확인할 도시
int k1, k2; 	// 도시의 ID
int speed;		// 자동차 속도 

int **graph;	// 인접 행렬 
int *visit; 	// 방문 배열
int *D;		// 경로 값  
int *P;	// 전 정점 값
int *pass;	// 이동 경로 값
char **city;	// 도시 이름 

// 인접행렬 초기화 
void initGraph();  

// file "map.txt" 에서 데이터를 입력받음
int readMap();
// file "city.txt" 에서 도시 이름을 입력받음  
int readCity();
// 경로 및 거리를 계산할 도시 입력 
int inputCity();
// ID(v1)에서 각 도시까지의 거리 계산 
void dfs(int v1);
// 출발 도시에서 도착 도시점까지의 이동 경로 출력  
void prnNode();


int main(){

	int i;
	
	// file "map.txt" 에서 데이터를 입력받음	
	readMap();
	// file "city.txt" 에서 도시 이름을 입력받음  
	readCity();	
	

	// 경로 및 거리글 계산할 도시(console)
	inputCity();

	D[k1] = 0;	// 시작 도시의 경로값은 0 
	P[k1] = MAX_INT;	// 시작점의 전 도시는 없음  
	
	// ID(v1)에서 각 도시까지의 거리 계산
	printf("\n<< 계산 중>>\n");	
	dfs(k1);
	
	if( D[k2] != MAX_INT )	prnNode();
	else {
		printf("\n========================================.\n");
		printf("두개의 도시가 연결되지 않았습니다.");
		printf("\n========================================.\n");
	}
	
	// 메모리 할당 해제 
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

// 인접행렬 초기화 
void initGraph(){
	int i, j;
	
	// **(2차포인터) 주소 할당 
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

	// 배열 초기화  
	for(i=0; i<n; i++) {
		for(j=0; j<n; j++)
			graph[i][j] = 0;
		visit[i] = 0;
		D[i] = MAX_INT;	// 모든 경로값은 최대값으로 설정  
		P[i] = 0;
		pass[i] = 0;
	}
}

// ID(v1)에서 각 도시까지의 거리 계산  
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
		putchar('.');	// 계산 중 화면에 '.' 출력 
	}
	
	for(i=0; i<n; i++) {
		for(j=0; j<n; j++) {
			if( graph[i][j]!=0 ) {
				if( (D[i]+graph[i][j]) < D[j] && D[i] != MAX_INT ) {
					D[j] = D[i] + graph[i][j];
					P[j] = i;	
				}
			}
		putchar('.');	// 계산 중 화면에 '.' 출력 
		}
	}	
	
}

// 출발 도시에서 도착 도시점까지의 이동 경로 출력  
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
	printf("\n이동 경로: " );	
	for(i=count; i>0; i--)
		printf("%s -> ", city[pass[i]] );
	printf("%s", city[pass[0]]);
	
	printf("\n총 이동거리 : %d", D[k2] );
	time = (float) D[k2] / speed;
	hour = (int) time;
	min = (int) ((time-(int)time) * 60);
	printf("\n속도 %dkm/h 로 도착 예상 시간 : %d시간 %d분", speed, hour, min);
	printf("\n========================================\n");
		
}

// file "city.txt" 에서 도시 이름을 입력받음
int readMap(){
	int i;
	int a, b, c;	// 입력 : 도시번호1, 도시번호2, 거리값  
	FILE *file;
	
	// "map.txt"파일 을 읽기 전용으로 연다 
	file = fopen("map.txt", "r");
	
	// 파일을 열지 못하면 메시지 출력 후 프로그램을 종료한다.		
	if( file == NULL )	{
		printf("File(map.txt) not found.\n");
		exit(1);
	}

	fscanf(file, "%d %d", &n, &m); 	// 입력: 도시의 수, 도시 관계의 수 

	// 인접행렬 초기화 
	initGraph();

	for(i=0; i<m; i++){
		fscanf(file, "%d %d %d", &a, &b, &c);	// 간선 번호 입력, 가중치 입력    
		graph[a][b] = graph[b][a] = c; 	// 인접 행렬에 가중치 표시   
	}

	fclose(file);
	 
	return 0;
}

// file "city.txt" 에서 도시 이름을 입력받음  
int readCity() {
	int i;
	FILE *file;
	
	// "city.txt"파일 을 읽기 전용으로 연다 
	file = fopen("city.txt", "r");
	
	// 파일을 열지 못하면 메시지 출력 후 프로그램을 종료한다.
	if( file == NULL )	{
		printf("File(city.txt) not found.\n");
		
		// 메모리 할당 해제 
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

// 경로 및 거리를 계산할 도시 입력
int inputCity(){
	int chk1=0, chk2=0;
	int i;
	
	printf("거리 및 경로를 계산할 2개의 도시 이름을 입력해주세요 "); 
	printf("\n도시 이름은 공백으로 띄워주세요."); 

	do {
		chk1 = chk2 = 0;
		printf("\n출발도시 도착도시: ");
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
			printf("   => Error! city.txt 파일 내용에 없는 도시이름을 입력하셨습니다. 다시 입력하여 주세요.");
	} while( chk1==0 || chk2==0 );
		
	while(1) {
		printf("\n이동 속도를 입력해주세요.\n");
		printf("1 ~ 200 사이의 정수만 입력해주세요 \n");
		printf("속도(km/h) : ");
		scanf("%d", &speed); 
		if (speed>1 && speed<=200) break;
	}

	return 0;	
}

