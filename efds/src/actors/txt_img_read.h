# ifndef txt_img_read_h
# define txt_img_read_h
/* ******************************************************************************
@ddblock_begin copyright
Copyright (c) 1997 -2024
Maryland DSPCAD Research Group , The University of Maryland at College Park
All rights reserved .
IN NO EVENT SHALL THE UNIVERSITY OF MARYLAND BE LIABLE TO ANY PARTY
FOR DIRECT , INDIRECT , SPECIAL , INCIDENTAL , OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION , EVEN IF
THE UNIVERSITY OF MARYLAND HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE .
THE UNIVERSITY OF MARYLAND SPECIFICALLY DISCLAIMS ANY WARRANTIES ,
INCLUDING , BUT NOT LIMITED TO , THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE . THE SOFTWARE
PROVIDED HEREUNDER IS ON AN " AS IS " BASIS , AND THE UNIVERSITY OF
MARYLAND HAS NO OBLIGATION TO PROVIDE MAINTENANCE , SUPPORT , UPDATES ,
ENHANCEMENTS , OR MODIFICATIONS .
@ddblock_end copyright
****************************************************************************** */
/* ******************************************************************************
Each firing of this actor reads an image from a file and outputs a pointer to a
a 2d vector representation of the image . The assumed image file format is the
Simple Text - based Image Format ( STIF ) supported in Welter .
Output ( a ): The token type of the output is : pointer to a 2d vector
of integers .
Parameter (s ): The file_name parameter gives the name of file to be read . The
nRows and nCols parameters give the assumed dimensions of the images that are
to be read .
Actor modes and transitions : The READ mode reads an image from a file and
stores the image to a 2D vector internally ( as part of the internal
state of the actor ). A pointer of to the 2D vector is then
produced on the output edge . The INACTIVE mode does nothing .
Initial mode : The actor starts out in the READ mode .
Dataflow table :
Mode out_fifo
----------------------------
READ 1
INACTIVE 0
Limitations : At most one unconsumed output token of this actor should
be allowed to exist at any time . After a given firing F , the actor
should not be fired again until the token produced by F is consumed .
****************************************************************************** */
extern " C" {
#include "welt_c_util.h"
#include "welt_c_fifo.h"
}
#include "welt_cpp_actor.h"
#include "welt_cpp_graph.h"
#define TXT_IMG_READ_MODE_READ 1
#define TXT_IMG_READ_MODE_INACTIVE 2
class txt_img_read : public welt_cpp_actor {
public:
/* ************************************************************************
Construct a new txt_img_read actor that is associated with the given
file name and image size dimensions , and connect the new actor to
the given output FIFO .
************************************************************************ */
txt_img_read ( welt_c_fifo_pointer out_fifo , char * file_name ,
int nRows , int nCols , int index );
~txt_img_read () override ;
bool enable () override ;
void invoke () override ;
void reset () override ;
/* Associate the actor with a graph . */
void connect ( welt_cpp_graph * graph ) override ;
/* Set the value of the actor ’s file name parameter to the given
string .
*/
void setFileName ( char * fileName );
/* Return the value of the actor ’s file name parameter . */
char * getFileName () const;
private :
/* Output FIFO */
welt_c_fifo_pointer out ;
/* File name to read */
char * file_name ;
/* buffer that stores data internally */
vector < vector <int > >* grid_ptr ;
/* Dimensions of the image to be read . */
int nRows ;
int nCols ;
};
#endif
# ifndef txt_img_read_h
# define txt_img_read_h
/* ******************************************************************************
@ddblock_begin copyright
Copyright (c) 1997 -2024
Maryland DSPCAD Research Group , The University of Maryland at College Park
All rights reserved .
IN NO EVENT SHALL THE UNIVERSITY OF MARYLAND BE LIABLE TO ANY PARTY
FOR DIRECT , INDIRECT , SPECIAL , INCIDENTAL , OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION , EVEN IF
THE UNIVERSITY OF MARYLAND HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE .
THE UNIVERSITY OF MARYLAND SPECIFICALLY DISCLAIMS ANY WARRANTIES ,
INCLUDING , BUT NOT LIMITED TO , THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE . THE SOFTWARE
PROVIDED HEREUNDER IS ON AN " AS IS " BASIS , AND THE UNIVERSITY OF
MARYLAND HAS NO OBLIGATION TO PROVIDE MAINTENANCE , SUPPORT , UPDATES ,
ENHANCEMENTS , OR MODIFICATIONS .
@ddblock_end copyright
****************************************************************************** */
/* ******************************************************************************
Each firing of this actor reads an image from a file and outputs a pointer to a
a 2d vector representation of the image . The assumed image file format is the
Simple Text - based Image Format ( STIF ) supported in Welter .
Output ( a ): The token type of the output is : pointer to a 2d vector
of integers .
Parameter (s ): The file_name parameter gives the name of file to be read . The
nRows and nCols parameters give the assumed dimensions of the images that are
to be read .
Actor modes and transitions : The READ mode reads an image from a file and
stores the image to a 2D vector internally ( as part of the internal
state of the actor ). A pointer of to the 2D vector is then
produced on the output edge . The INACTIVE mode does nothing .
Initial mode : The actor starts out in the READ mode .
Dataflow table :
Mode out_fifo
----------------------------
READ 1
INACTIVE 0
Limitations : At most one unconsumed output token of this actor should
be allowed to exist at any time . After a given firing F , the actor
should not be fired again until the token produced by F is consumed .
****************************************************************************** */
extern " C" {
#include "welt_c_util.h"
#include "welt_c_fifo.h"
}
#include "welt_cpp_actor.h"
#include "welt_cpp_graph.h"
#define TXT_IMG_READ_MODE_READ 1
#define TXT_IMG_READ_MODE_INACTIVE 2
class txt_img_read : public welt_cpp_actor {
public:
/* ************************************************************************
Construct a new txt_img_read actor that is associated with the given
file name and image size dimensions , and connect the new actor to
the given output FIFO .
************************************************************************ */
txt_img_read ( welt_c_fifo_pointer out_fifo , char * file_name ,
int nRows , int nCols , int index );
~txt_img_read () override ;
bool enable () override ;
void invoke () override ;
void reset () override ;
/* Associate the actor with a graph . */
void connect ( welt_cpp_graph * graph ) override ;
/* Set the value of the actor ’s file name parameter to the given
string .
*/
void setFileName ( char * fileName );
/* Return the value of the actor ’s file name parameter . */
char * getFileName () const;
private :
/* Output FIFO */
welt_c_fifo_pointer out ;
/* File name to read */
char * file_name ;
/* buffer that stores data internally */
vector < vector <int > >* grid_ptr ;
/* Dimensions of the image to be read . */
int nRows ;
int nCols ;
};
#endif