#include "Score.h"

void Score ::SetScore(int sc)
{
	 score = sc ;

}
void Score ::Init(const char * path, char* file)
{
	FILE  *f;

	int pathLength = strlen(path);
	int fileLength = strlen(file);
	char * fullPath = new char[pathLength + fileLength + 1];
	strcpy(fullPath, path);
	strcpy(fullPath + pathLength, file);

	f = fopen(fullPath,"r");

	SAFE_DEL_ARRAY(fullPath);

	if( f == NULL)
	{
	     LOGI("\n  Read file error!");
		 return ;
	}
	int numberScoreType = 0;
    char  IDscoreType[100] ;
	fscanf(f,"\n#ScoreType %d",&numberScoreType);
	scoreType = new int[numberScoreType];
	char *stringID = "\nID %s";
	char *stringScore = "\nScore %d";
	for(int i=0; i<numberScoreType; i++)
	{
		int index = 0;
		fscanf(f, stringID, IDscoreType);
		if ( strcmp(IDscoreType, "LargeChicken")  == 0)
		{
			index = LARGE_CHICKEN;
		}
		else if ( strcmp(IDscoreType, "SmallChicken") == 0 )
		{
			index = SMALL_CHICKEN;
		}
		else if( strcmp(IDscoreType, "ThighChicken") == 0 )
		{
			index = THIGH_CHICKEN;
		}
		fscanf(f,stringScore, &scoreType[index]);

	}	
	
}
void Score :: ScoreUp(int scoreId)
{
	score += this->scoreType[scoreId];
}

void  Score ::GetHighScore(const char * path, char *file)
{
	FILE  *f;

	int pathLength = strlen(path);
	int fileLength = strlen(file);
	char * fullPath = new char[pathLength + fileLength + 1];
	strcpy(fullPath, path);
	strcpy(fullPath + pathLength, file);
	
	f = fopen(fullPath,"r");

	SAFE_DEL_ARRAY(fullPath);

	if( f == NULL)
	{
	     LOGI("\n  Read file error!");
		 return;
	}
	fscanf(f,"\n#HighScore %d",&numberHighScore);
	char *stringIDHighScore = "\nID %d";
	char *stringhighscore = "\nScore %d";
	int IDhighscore =0;
	highScore= new int[numberHighScore];
	for(int i=0;i<numberHighScore;i++)
	{
		fscanf(f,stringIDHighScore,&IDhighscore);
		fscanf(f,stringhighscore,&highScore[i]);

	}  
	for(int i=0;i< numberHighScore; i++)
	{
		LOGI("\n%d",highScore[i]);
	}

}
void Score ::SetHighScore(const char * path, char *file)
{
     

	this->GetHighScore(path,file);
	this ->SetScore(50002);
	
	highScore[numberHighScore]= score;
	numberHighScore = numberHighScore + 1;
	for(int i=0;i<numberHighScore-1;i++)
	 {
		 for(int j=i+1;j<numberHighScore;j++)
		 {
			 if(highScore[i] < highScore[j])
			 {
				 int temp = highScore[i];
			     highScore[i] = highScore[j];
				 highScore[j] = temp;
			 }
		 }
	 }


	FILE *f ;
	int pathLength = strlen(path);
	int fileLength = strlen(file);
	char * fullPath = new char[pathLength + fileLength + 1];
	strcpy(fullPath, path);
	strcpy(fullPath + pathLength, file);
	
	
	f = fopen(fullPath,"w+");
	SAFE_DEL_ARRAY(fullPath);
	if( f == NULL)
	{
	     LOGI("\n  Read file error!");
		 return;
	}

	//if(numberHighScore < 5)
	//{

	if(numberHighScore>5)
	{
		numberHighScore = 5;
	}
	fprintf(f,"\n#HighScore %d",numberHighScore );
	char *stringIDHighScore = "\nID %d";
	char *stringhighscore = "\nScore %d";

	for(int i=0;i<numberHighScore ;i++)
	{
		fprintf(f,stringIDHighScore, i);
		fprintf(f,stringhighscore, highScore[i]);

	}  
	
	for(int i=0;i< numberHighScore; i++)
	{
		LOGI("\n%d",highScore[i]);
	}


	
}

Score::Score(void)
{
	
}

Score::~Score(void)
{
}
