#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>
#define  NUM_OF_CLASSIFIERS 10
#define  VARIBLES 3
#define  CLASSIFIER_0 "/classifier_0.csv"
#define  CLASSIFIER "/classifier_"
#define  DATASET "/dataset.csv"
#define  LABELS "/labels.csv"
#define  CSV ".csv"

using namespace std;

struct classifiers
{
    vector<long double> Betha_0;
    vector<long double> Betha_1;
    vector<long double> Bias;

};

void open_file_dataset(vector<long double> &width, vector<long double> &length,string address)
{
	string str;
	ifstream fin;
	fin.open(address);
    getline(fin, str, ',');
    getline(fin, str, '\n');
	while (getline(fin, str, ','))
	{

	    length.push_back(stold(str));
		getline(fin, str, '\n');
		width.push_back(stold(str));
	}
}

vector<long double>open_file_labels(string address,vector<long double> &labels)
{
    string str;
    ifstream fin;
	fin.open(address);
    getline(fin, str, '\n');
    while (getline(fin, str, '\n'))
	   labels.push_back(stold(str));

    return labels;
}

void open_file_class(string address,struct classifiers &flower)
{
    string str;
    ifstream fin;
	fin.open(address);
    getline(fin, str, ',');
    getline(fin, str, ',');
    getline(fin, str, '\n');
	while (getline(fin, str, ','))
	{
	    flower.Betha_0.push_back(stold(str));
        getline(fin, str, ',');
	    flower.Betha_1.push_back(stold(str));
        getline(fin, str, '\n');
	    flower.Bias.push_back(stold(str));  
	}

}

int count_line_classifier(string address)
{
    int count_lines;
    string str;
    ifstream fin;
	fin.open(address);
    getline(fin, str, ',');
    getline(fin, str, ',');
    getline(fin, str, '\n');
	while (getline(fin, str, ','))
	{
        getline(fin, str, ',');
        getline(fin, str, '\n');
        count_lines++;
	}
    return count_lines;
}

int find_bigest_result ( long double width,long double length,struct classifiers  &flower)
{
    vector<long double>score;
    
     long double scores;
    for(int i=0;i<VARIBLES;i++)
    {
        scores=(flower.Betha_0[i]*length)+(flower.Betha_1[i]*width)+flower.Bias[i];
        score.push_back(scores);
    }
    long double max = score[0];
    for(int j=1;j<VARIBLES;j++)
    {
        if(score[j]>max)
            max = score[j];
    }
    
    long double index;
    for(int i=0;i<VARIBLES;i++)
    {
        if(score[i]==max)
            index=i;
    }
    return index; 
}

int find_main_results(vector<long double>result_classes,int count_lines)
{
    
    vector<int>numbers_repeated_result;
    int counter=0;
    for(int j=0;j<count_lines;j++)
    {  
        counter =0;
        for(int i=0;i<NUM_OF_CLASSIFIERS;i++)
        {
            if(result_classes[i] == j)
            counter++;
         }
         numbers_repeated_result.push_back(counter);
    }

    int max = numbers_repeated_result[0];
    int index=0;
    for(int j=0;j<count_lines;j++)
    {
        if(numbers_repeated_result[j]>max)
        {
            max = numbers_repeated_result[j];
            index=j;
        }
    }
    return index;
}

vector<long double>remove_result_classes(vector<long double>&result_classes)
{
    for(int k=0;k<NUM_OF_CLASSIFIERS;k++)
        result_classes.pop_back();
    
    return result_classes;
}

void process_datas(string keep_argv_1,vector<long double> &length,vector<long double> &width,vector<string> &classifier_addresses,vector<classifiers> &classifiers_num,vector<long double> &result_classes,vector<int> &main_results,int count_lines)
{
    for(int j=0;j<width.size();j++)
    {
        for (int i=0;i<NUM_OF_CLASSIFIERS;i++)
        {
            string adress_file = keep_argv_1 + CLASSIFIER + to_string(i) + CSV;
            classifier_addresses.push_back(adress_file);
            classifiers temp;
            classifiers_num.push_back(temp);
            open_file_class(classifier_addresses[i],classifiers_num[classifiers_num.size()-1]);
            int result_class = find_bigest_result (width[j], length[j], classifiers_num[classifiers_num.size()-1]);
            result_classes.push_back(result_class);
        }
            int main_result= find_main_results(result_classes,count_lines);
            main_results.push_back(main_result);
            remove_result_classes(result_classes);
    }
}

float round(float var) 
{ 
    char str[40];  
    sprintf(str, "%.2f", var); 
    sscanf(str, "%f", &var);  
    return var;  
}
void compare_and_print_percent( vector<long double>width,vector<int>main_results,vector<long double>labels)
{
    double correct_count=0;
    for(int k=0;k<width.size();k++)
    {
        if(labels[k]==main_results[k])
            correct_count++;
    }
    float percent = 100*((long double)correct_count/(long double)width.size()); 
    char str[40];
    percent = round(percent);
    sprintf(str, "%.2f", percent);
    cout<<"Accuracy: " <<str<<"%"<<endl;
}

int main(int argc, char *argv[])
{
    vector<long double>width;
    vector<long double>length;
    vector<long double>labels;
    vector<long double>result_classes;
    vector<int>main_results;
    vector<classifiers> classifiers_num;
    string keep_argv_1 = argv[1];
    keep_argv_1+=DATASET;
    open_file_dataset(width,length,keep_argv_1);
    string keep_argv = argv[1];
    keep_argv+=LABELS;
    string keep_argv_2 = argv[2];
    vector<string>classifier_addresses;
    string address_0=keep_argv_2+CLASSIFIER_0;
    int count_lines=count_line_classifier(address_0);
    process_datas(keep_argv_2,length,width,classifier_addresses,classifiers_num,result_classes,main_results,count_lines);
    open_file_labels(keep_argv,labels);
    compare_and_print_percent(width,main_results,labels);

    return 0;
}