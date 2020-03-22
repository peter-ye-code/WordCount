#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<io.h> 

int getChar(FILE *fp);
int getWord(FILE *fp);
int getLine(FILE *fp);
int getnull_line(FILE *fp);
int getcomplex_data(FILE *fp,int &annotion_line,int &code_line);
int getother_line(FILE *fp);
int handle_file();

int main(int argc,char*argv[]){
	FILE *fp;
	int number;
	char string;

    if(argc==3){
    	
    	//��ȡ�ļ�������
		fp = fopen(argv[2],"r");
		
		//��ʧ�� 
		if(fp == NULL){
			printf("�����޷���%s\n",argv[2]);
			exit(1);
		} 
			//�Բ�������ƥ�� 
		    if(!strcmp(argv[1],"-c")){//ͳ���ַ��� 
		        number = getChar(fp);
		     	printf("�ַ���Ϊ%d\n",number);
		    }
		
		    else if(!strcmp(argv[1],"-w")){//ͳ�Ƶ����� 
			    number = getWord(fp);
			    printf("������Ϊ%d\n",number); 
		   }
		
		    else if(!strcmp(argv[1],"-l")){//ͳ������ 
			   number = getLine(fp);
			    printf("����Ϊ%d\n",number);
	        }	
			
			else if(!strcmp(argv[1],"-a")){			
				printf("��������Ϊ%d\n",getnull_line(fp));
				int a,b;
				getcomplex_data(fp,a,b);
				printf("ע������Ϊ%d\n",a);
				printf("��������Ϊ%d\n",b);
			}
			else if(!strcmp(argv[1],"-s")){
			     handle_file();
			}
	}
	
	else {
		printf("���󣺲��������������������\n");
	}
	
} 

//ͳ���ַ��� 
int getChar(FILE *fp){
	char ch;
	int number=0;
	while((ch=fgetc(fp))!=EOF){//������fgetc���ص�ASCIIֵ��Ϊ EOF����ʾ�����Լ����� 
		if((ch!='\n')&&(ch!=' ')&&(ch!='\t')){
			number++;
		}
	}
	rewind(fp);
	return number;
}

//ͳ�Ƶ����� 
int getWord(FILE *fp){
	int flag=0;
	int number=0;
	char ch;
	while(!feof(fp))
    {
        ch = fgetc(fp);
        if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'))
            flag = 0;
        else if(flag == 0 && (ch != '-' && ch != '/'&& ch != '\'')) //����������ѭ�����������������ѭ���������صĲ�����ĸ�������� 
        {
            number++;
            flag = 1;
        }
    }
    rewind(fp);
    return number;
}

//ͳ������ 
int getLine(FILE *fp){
	char ch;
	int number=0;
	while(feof(fp)==0)//����0��ʾ�ļ�δ���� 
	{
		ch = fgetc(fp);//��ȡһ���ַ�����ȡһ���ֽں󣬹��λ�ú���һ���ֽ�
		if(ch=='\n'){//����ǻ��з� 
			number++;
		}
	}
	rewind(fp);
	return number+1;
}

//ͳ�ƿ���
int getnull_line(FILE *fp){
	int null_line = 0;
	int bufsize = 100;
	char buf[bufsize] = {0};
	while(feof(fp)==0){
		//�����ļ���ÿһ�� 
		fgets(buf,bufsize,fp); 
		if(buf[0]=='\n'){
			null_line++;
		}		
	}
	rewind(fp);
	return null_line;
} 
 
//ͳ������ע����
int getother_line(FILE *fp){
	int line;
	while(feof(fp)==0){
	 	char buf[100] = {0};
	    fgets(buf,100,fp);
	    int len=strlen(buf);  
	    for(int i=0;i<len;i++){
	    	if (buf[i]!='/')
    			{
    				if ((i+2<len) && (buf[i+1]=='/')&&(buf[i+2]=='/')) 
    				line++;	
    			}
		}
	} 
	rewind(fp);
	return line;
} 

//ͳ��ע���кʹ����� 
int getcomplex_data(FILE *fp,int &annotion_line,int &code_line){
	int sigle_annotion_line = 0;
	int bufsize = 100;
	char buf[bufsize] = {0};
	int flag = 0;
	int multi_annotion_line=0;
	while(feof(fp)==0){ 
    	fgets(buf,bufsize,fp);		
		if(buf[0]=='/'&&buf[1]=='/'){
			sigle_annotion_line++;
		}else if(flag == 0){		
			if(buf[0]=='/'&&buf[1]=='*'){
			    multi_annotion_line++;
			    flag=1;
			}		   
	    }else if(flag == 1){
			if(buf[0]!='*'&&buf[1]!='/'){
				multi_annotion_line++;
			}else if(buf[0]=='*'&&buf[1]=='/'){
				multi_annotion_line++;
				flag=0;
			}
		}
	}
	//�ڵ���һ�������轫�ļ�ָ������Ὺͷ 
	rewind(fp);
	int other_annotion_line = getother_line(fp);
	annotion_line = sigle_annotion_line+multi_annotion_line+other_annotion_line;
	int line = getLine(fp);
	int null_line = getnull_line(fp);
	code_line = line - annotion_line - null_line;
}


//����Ŀ¼�·����������ļ�
int handle_file(){   
    struct _finddata_t fileinfo;    //�ļ��洢��Ϣ�ṹ�� 
    long file_handle;   //������ļ����
    file_handle = _findfirst("*.txt",&fileinfo);    //���ҵ�ǰĿ¼�µ�txt�ļ�
    FILE *fp;
    int a,b;
    if(file_handle==-1)
        printf( "��ǰĿ¼��û��txt�ļ�\n");
    else 
        do{//���ҵ�һ��������һ�� 
            fp=fopen(fileinfo.name,"r");
            printf("�ҵ��ļ�%s\n",fileinfo.name);
            printf("�ַ���Ϊ%d\n",getChar(fp));
            printf("����Ϊ%d\n",getWord(fp));
            printf("����Ϊ%d\n",getLine(fp));
            printf("������Ϊ%d\n",getnull_line(fp));
            getcomplex_data(fp,a,b);
            printf("ע������Ϊ%d\n",a);
			printf("��������Ϊ%d\n\n",b);
        }while(_findnext(file_handle,&fileinfo)==0);//�ҵ���һ�� 
        _findclose(file_handle);    //�ر��ļ����� 
}





