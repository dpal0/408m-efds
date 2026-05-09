#include <iostream>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "../../src/actors/classifierActor.h"
using namespace std;
#define CAPACITY 3
vector<vector<int>> Img_to_vector(string filename){
    ifstream img(filename);
    vector<vector<int>> pix(24,vector<int>(24));
    if(!img.is_open()){
        cerr<<"can't open image file"<<endl;
        return pix;
    }
    string line;
    int row=0;
    while(getline(img,line)){
        stringstream ss(line);
        string val;
        int col=0;
        while(getline(ss,val,',') && col<24){
            pix[row][col]=stoi(val);
            col++;
        }
        row++;
    }
    return pix;
}
bool startClassifier(string imgPath, string configPath){
    welt_c_fifo_pointer in=welt_c_fifo_new(CAPACITY,sizeof(IIS*),0);
    welt_c_fifo_pointer abort_out=welt_c_fifo_new(CAPACITY,sizeof(IIS*),0);
    welt_c_fifo_pointer cont_out=welt_c_fifo_new(CAPACITY,sizeof(IIS*),0);
    vector<vector<int>> pix=Img_to_vector(imgPath);
    PTIS *image=new PTIS();
    image->pix=pix;
    IIS *token=new IIS();
    token->I=image;
    token->k=1;
    welt_c_fifo_write(in,&token);
    classifierActor *stage=new classifierActor(in, abort_out, cont_out,configPath);
    for(int i=0; i<10; i++){
        if(stage->enable()){
            stage->invoke();
        }
    }
    bool detectedFace=false;
    if(welt_c_fifo_population(cont_out)>0){
        IIS* outToken;
        welt_c_fifo_read(cont_out,&outToken);
        detectedFace=true;
    }
    delete stage;
    return detectedFace;
}
int main(){
    for(int config=0; config<=3; config++){
        string configPath="../configs/config"+to_string(config)+".txt";
        cout<<"_________Test Config"<<config<<"____________"<<endl;
        int faceFails=0;
        int nonFaceFails=0;
        for(int idx=1; idx<=4; idx++){
            string imgPath="../face/000"+to_string(idx)+".txt";
            bool face=startClassifier(imgPath,configPath);
            if(face){
                cout<<"Face "<<idx<<" detected as Face\n";
            }else{
                cout<<"Face "<<idx<<" detected as Not Face\n";
                faceFails++;
            }
        }
        for(int idx=1; idx<=4; idx++){
            string imgPath="../non-face/000"+to_string(idx)+".txt";
            bool face=startClassifier(imgPath,configPath);
            if(!face){
                cout<<"NonFace "<<idx<<" detected as NonFace\n";
            }else{
                cout<<"NonFace "<<idx<<" detected as Face\n";
                nonFaceFails++;
            }
        }
        cout<<"Config "<<config<<"  ____Summary____\n";
        if(faceFails==0){
            cout<<"all faces passed\n";
        }else{
            cout<<"number of faces failed: "<<faceFails<<endl;
        }
        if(nonFaceFails==0){
            cout<<"all nonfaces passed\n";
        }else{
            cout<<"number of nonfaces failed: "<<nonFaceFails<<endl;
        }
    }
    return 0;
}