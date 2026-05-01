#include <fstream>
#include <vector>
#include <string>
#include "classifierActor.h"
#include "integral_image.h"
#include <iostream>

using namespace std;
classifierActor::classifierActor(welt_c_fifo_pointer in,welt_c_fifo_pointer abort_out,
                                 welt_c_fifo_pointer cont_out, int stage){
    this->in=in;
    this->abort_out=abort_out;
    this->cont_out=cont_out;
    this->stage=stage;
    this->mode=CONFIGURE;
    this->sc=nullptr;
    this->currToken=nullptr;
}
bool classifierActor::enable(){
    bool result=false;
    switch(mode){
        case CONFIGURE: //load config file stuff
            result=true;
            break;
        case READ: //read image
            result=(welt_c_fifo_population(in)>0);
            break;
        case CLASSIFY: // as face or not face
            result=true;
            break;
        case TRUE_MOD: //is face
            result=(welt_c_fifo_population(cont_out)<welt_c_fifo_capacity(cont_out));
            break; 
        case FALSE_MOD: //not face
            result=(welt_c_fifo_population(abort_out)<welt_c_fifo_capacity(abort_out));
            break;
        default:
            result=false;
            break;
    }
    return result; 
}

void classifierActor::invoke(){
    switch(mode){
        case CONFIGURE: { //load in data from config file to create strong classifier
            ifstream configFile("config"+to_string(stage) + ".txt"); //opens config file with current stage
            if(!configFile.is_open()){ //when configFile can't be opened. 
                std::cerr<<"config file error when open\n";
                //mode=FALSE_MOD; Lets not change to False when config fails -Diksha
                break;
            }
            //used later to store values from config file. 
            float sc_threshold,weight,wc_threshold,error;
            //weight= one weak classifier weight
            //wc_threshold=one weak classifier threshold
            //error= error of one weak classifier after training it. 
            int polarity,x,y,w,h,type_int;
            //polarity=polarity of the weak classifier after training it. 
            //x,y = top and left shows rectangle location
            // w,h=width and height shows size of the haar feature.
            vector<wc_weight_pair> wc_vector; //store all weak classifier for current stage
            configFile >>sc_threshold; //reads the strong classifier threshold store into sc_threshold. 
            while(configFile>>x>>y>>w>>h>>type_int>>wc_threshold>>polarity>>error>>weight){
                // iterate through configFile to store each weak classifier trained values 
                Feature type=(Feature) type_int; //converts int into Feature enum. 
                HaarFeature hf(x,y,w,h,type); //values from configFile used to create haarfeature.
                WeakClassifier *wc=new WeakClassifier(hf,wc_threshold,polarity,error); // used created hf and values from config to create weakclassifier.
                wc_weight_pair pair; //pair object contain weak classifer and weight
                pair.wc=wc; //weak classifier object
                pair.weight=weight; //weight of weak classifier
                wc_vector.push_back(pair); //put pair of weak classifier into weak classifier vector/list.
            }
            configFile.close(); 
            sc=new StrongClassifier(wc_vector,sc_threshold); //create StrongClassifier with list of weak classifier and strong classifier threshold
            mode=READ; 
            break;
        }
        case READ: { //read input 
            IIS *token; //used to store input token pair (I image window,K image index)
            welt_c_fifo_read(in,&token); //store input token pair in currToken pair
            this->currToken=token; //put into private currToken, used later in Classify function
            mode=CLASSIFY; 
            break;
        }
        case CLASSIFY:{ //classify as face or non-face
            PTIS *imgI=this->currToken->I; //get img from token
            IntegralImage ii(imgI->pix); //calls integral image function to convert image to integral image
            if(sc==nullptr){
                mode=FALSE_MOD;
                break;
            }
            bool face=sc->classify(ii.getImage()); //created intImg used to classify face or non face. 
            if(face){ //face==1 is found mode=true
                mode=TRUE_MOD;
            }else{ //non face is found, mode=false
                mode=FALSE_MOD;
            }
            break;
        }
        case TRUE_MOD:{
            IIS *copyToken=new IIS(*this->currToken);
            welt_c_fifo_write(cont_out, &copyToken); //writes the (I,K) token and send it to the next actor
            mode=READ;
            break;
        }
        case FALSE_MOD:{
            IIS *copyToken=new IIS(*this->currToken);
            welt_c_fifo_write(abort_out,&copyToken); //writes to abort output to send (I,K) as non-face. 
            mode=READ;
            break; //Diksha added
        }
        default:
            break;
    }
}
void classifierActor::reset(){ //reset classifier actor
    mode=CONFIGURE;
}
classifierActor::~classifierActor(){ //destructor
    delete sc;
}
void classifierActor::connect(welt_cpp_graph *graph){ 
    int port_index;
    int direction;
    direction=GRAPH_IN_CONN_DIRECTION;
    port_index=0;
    graph->add_connection(this,port_index,direction); //input
    direction=GRAPH_OUT_CONN_DIRECTION;
    port_index=0; //out
    graph->add_connection(this,port_index,direction);
    direction=GRAPH_OUT_CONN_DIRECTION;
    port_index=1; //out 
    graph->add_connection(this,port_index,direction);

}