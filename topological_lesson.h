#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#define MAX_SIZE 100
using namespace std;

struct Lesson
{
    string index="\0";
    string name;
    int lesson_hour;
    int assigned_semester;
    vector<string> pre_lessons;
};

struct Node
{
    int vertex;
    Node *next=nullptr;
};

struct Headnode
{
    string index;
    Node *link=nullptr;
};

class topological_lesson
{
private:
    int n[8]={0};//每个学期的课程数
    char filename[MAX_SIZE];
    int count[MAX_SIZE]={0};//节点入度
    int h;//除去已指定学期课后剩下的课程数
    Headnode head[MAX_SIZE];
    vector<Lesson> lessons;//拓扑排序前的课程
    vector<string> topological_lessons;//拓扑排序后的课程编号
public:
    void load()//将课程信息存储在vector中
    {
        int j=0; 
        cout<<"Please enter the filename of the file you operate"<<endl;
        cout<<"ATTENTION:the file must be in the same folder of the code"<<endl;
        cin>>filename;
        ifstream infile(filename);  
        if (!infile.is_open()) {
            cout<<"There is no existed file"<<endl;
            exit(EXIT_FAILURE);
        }
        string line;
        while (getline(infile, line)) { 
            istringstream iss(line); 
            Lesson lesson;
            if (!line.empty()&&line.substr(0, 2) != "//"&&j==0)
            {
                for(j;j<=7;j++) iss>>n[j];//首先将第一个非空行和注释行的内容装进课程数数组里
            }
            if (iss >> lesson.index >> lesson.name >> lesson.lesson_hour >> lesson.assigned_semester) //再将其他行装进lesson里
            {
                string pre_lesson;
                while(iss>>pre_lesson)
                lesson.pre_lessons.push_back(pre_lesson);
                lessons.push_back(lesson);  
            }
        }
        infile.close();  
    }

    void graph_generate()//生成图的邻接表
    {
        int n=0,q=0;
        for(int m=0;m<lessons.size();m++)//将需要计算机安排的课程装入顶点节点
        {
            if(lessons[m].assigned_semester==0) 
            {   
                head[n].index=lessons[m].index;
                n++;
            }
        } 
        h=n+1;
        for(int p=0;p<lessons.size();p++)//构建邻接表
        {   
            if(lessons[p].pre_lessons.size()!=0)
            {
                for(int o=0;o<lessons[p].pre_lessons.size();o++)
                {    
                    q=0;
                    Node *temp1=new Node();
                    while(head[q].index!=lessons[p].pre_lessons[o]) 
                        q++;
                    temp1->vertex=p;
                    temp1->next=nullptr;
                    if(head[q].link==nullptr)   
                        head[q].link=temp1;
                    else 
                    {
                        Node *temp2=head[q].link;
                        while(temp2->next!=nullptr)    
                            temp2=temp2->next;
                        temp2->next=temp1;
                    } 
                    count[p]++;   
                }   
            }
        }
    }

    void topological_sort()//拓扑排序
    {
        int top=-1,j;   
        for(int i=h-2;i>=0;i--)
        {
            if(count[i]==0)
            {
                count[i]=top;
                top=i;
            }
        }
        while(top!=-1)//一直将入度为0的点出栈，并进入拓扑排序后的vector
        {   
            j=top;
            top=count[top];
            cout<<head[j].index<<" ";
            topological_lessons.push_back(head[j].index);
            if(head[j].link!=nullptr)
            {
                Node* temp=head[j].link;
                count[temp->vertex]--;
                if(count[temp->vertex==0])
                {
                    count[temp->vertex]=top;
                    top=temp->vertex;
                }
                while(temp->next!=nullptr)
                {
                    temp=temp->next;
                    count[temp->vertex]--;
                    if(count[temp->vertex]==0)
                    {
                        count[temp->vertex]=top;
                        top=temp->vertex;
                    }
                }
            }
        }
    }

    void print()//输出可能的选课顺序
    {
        int x=0;
        cout<<"可能的选课顺序如下："<<endl<<"第一学期：";
        for(int m=0;m<lessons.size();m++)//先输出指定学期的课程数
        {
            if(lessons[m].assigned_semester==1) 
            {   
                cout<<lessons[m].name<<" ";
                x++;
            }
        } 
        for(int a=0;a<(n[0]-x);a++)//第一学期还可选n[0]-x门课,优先输出没有先决条件的课程
        {
            topological_lessons;
        }
    }
};