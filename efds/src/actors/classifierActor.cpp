#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include "classifierActor.h"
#include "integral_image.h"
using namespace std;

//for debugging - diksha 5/5/202 2:38 pm
static string modeName(int m) {
    switch(m) {
        case CONFIGURE: return "CONFIGURE";
        case READ:      return "READ";
        case CLASSIFY:  return "CLASSIFY";
        case TRUE_MOD:  return "TRUE_MOD";
        case FALSE_MOD: return "FALSE_MOD";
        default:        return "UNKNOWN";
    }
}

classifierActor::classifierActor(welt_c_fifo_pointer in,welt_c_fifo_pointer abort_out,
                                 welt_c_fifo_pointer cont_out, string configFilePath){ //diksha
    this->in=in;
    this->abort_out=abort_out;
    this->cont_out=cont_out;
    this->configFilePath=configFilePath;
    this->mode=CONFIGURE;
    this->sc=nullptr;
    this->currToken=nullptr;
}

bool classifierActor::enable() {
    bool result = false;

    switch(mode){
        case CONFIGURE: //load config file stuff
            result = true;
            break;
        case READ: //read image
            result = (welt_c_fifo_population(in) > 0);
            break;
        case CLASSIFY: // as face or not face
            result = true;
            break;
        case TRUE_MOD: //is face
            result = (welt_c_fifo_population(cont_out) < welt_c_fifo_capacity(cont_out));
            break; 
        case FALSE_MOD: //not face
            result = (welt_c_fifo_population(abort_out) < welt_c_fifo_capacity(abort_out));
            break;
        default:
            result = false;
            break;
    }
    return result; 
}

void classifierActor::invoke() {

    switch(mode){

        case CONFIGURE: { 
            //cout<<"classifierActor is in \""<<modeName(mode)<<"\" //mode"<<endl;

            ifstream configFile(configFilePath);

            if(!configFile.is_open()) {
                cerr << "Error: couldnt open config file:" << configFilePath << endl;
            }

            float sc_threshold, weight, wc_threshold, error;
            int polarity, x, y, w, h, type_int;
            vector<StrongClassifier::wc_weight_pair> wc_vector; 
            
            configFile >> sc_threshold;

            while(configFile >> x >> y >> w >> h >> type_int >> wc_threshold >> polarity >> error >> weight){
                Feature type=(Feature) type_int;
                HaarFeature hf(x,y,w,h,type);
                WeakClassifier wc(hf,wc_threshold,polarity,error);

                StrongClassifier::wc_weight_pair pair(wc, weight);
                wc_vector.push_back(pair);
            }
            configFile.close(); 
            sc = new StrongClassifier(wc_vector,sc_threshold);

            mode=READ;
            break;
        }

        case READ: { 
            //cout<<"classifierActor is in \""<<modeName(mode)<<"\" mode"<<endl;

            IIS *token=nullptr; //used to store input token pair (I image window,K image index)
            welt_c_fifo_read(in,&token); //store input token pair in currToken pair

            if(token==nullptr){
                cerr<<"Error in mode "<<modeName(mode)<<": null token read from input"<<endl; //diksha 
                return;
            }

            this->currToken=token; //put into private currToken, used later in Classify function

            mode=CLASSIFY; 
            break;
        }
        case CLASSIFY:{ //classify as face or non-face
           //cout<<"classifierActor is in \""<<modeName(mode)<<"\" mode"<<endl;

            if(this->currToken==nullptr){
                cerr<<"Error in mode "<<modeName(mode)<<": currToken is null"<<endl;
                return;
            }

            // cout<<"currToken ok"<<endl;
            PTIS *imgI=this->currToken->I; //get img from token
            if(imgI==nullptr){
                cerr<<"Error in mode "<<modeName(mode)<<": imgI (image window) is null"<<endl;
                return;
            }

            // cout<<"imgI ok"<<endl;
            IntegralImage ii(imgI->pix); //calls integral image function to convert image to integral image
            //cout<<"integral image created"<<endl;
            if(sc==nullptr){
                cerr<<"Error in mode "<<modeName(mode)<<": strong classifier is null"<<endl;
                mode=FALSE_MOD;
                break;
            }
         
            //cout<<"before classify"<<endl;
            bool face=sc->classify(ii.getImage()); //created intImg used to classify face or non face. 
            if(face){ //face==1 is found mode=true
                mode=TRUE_MOD;
            }else{ //non face is found, mode=false
                mode=FALSE_MOD;
            }
            break;
        }
        case TRUE_MOD:{
            //cout<<"classifierActor is in \""<<modeName(mode)<<"\" mode"<<endl;
            if(this->currToken==nullptr){
                cerr<<"Error in mode "<<modeName(mode)<<": currToken is null"<<endl;
                return;
            }
            IIS *copyToken=new IIS();
            copyToken->k=this->currToken->k;
            copyToken->I=new PTIS();
            copyToken->I->pix=this->currToken->I->pix;
            welt_c_fifo_write(cont_out,&copyToken); //writes the (I,K) token and send it to the next actor
             // free original
            delete this->currToken->I;
            delete this->currToken;
            this->currToken = nullptr;
            mode=READ;
            break;
        }
        case FALSE_MOD:{
            //cout<<"classifierActor is in \""<<modeName(mode)<<"\" mode"<<endl;
            if(this->currToken==nullptr){
                cerr<<"Error in mode "<<modeName(mode)<<": currToken is null"<<endl;
                return;
            }
            IIS *copyToken=new IIS();
            copyToken->k=this->currToken->k;
            copyToken->I=new PTIS();
            copyToken->I->pix=this->currToken->I->pix;
            welt_c_fifo_write(abort_out,&copyToken); //writes to abort output to send (I,K) as non-face. 
             // free original
            delete this->currToken->I;
            delete this->currToken;
            this->currToken = nullptr;
            mode=READ;
            break;
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