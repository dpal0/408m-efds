#ifndef txt_img_write_h
#define txt_img_write_h
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
Overview : Each firing of this actor consumes a pointer to a 2D vector
representation of an image , and writes the image to a file in a text - based
image format . The image file format that is utilized is the Simple Text - based
Image Format ( STIF ) supported in Welter .
Input (s ): The token type of the input is : pointer to a 2d vector
of integers .
Parameter (s ): The file_name parameter gives the name of file to be written
to .
Actor modes and transitions : The WRITE mode reads an image from the input
FIFO , and writes the image to the file that is specifed by the file_name
parameter . The INACTIVE mode does nothing .
Dataflow table :
Mode in_fifo
----------------------------
WRITE -1
INACTIVE 0
Initial mode : The actor starts out in the WRITE mode .
****************************************************************************** */
extern " C" {
#include "welt_c_util.h"
#include "welt_c_fifo.h"
}
#include "welt_cpp_actor.h"
#include "welt_cpp_graph.h"
#define TXT_IMG_WRITE_MODE_WRITE 1
#define TXT_IMG_WRITE_MODE_INACTIVE 2

class txt_img_write : public welt_cpp_actor {
public:
/* ************************************************************************
Construct a new txt_img_write actor that is associated with the given
file name , and connect the new actor to the given input FIFO .
************************************************************************ */
txt_img_write ( welt_c_fifo_pointer in_fifo , char * file_name ,
int index );
~ txt_img_write () override ;
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
/* Input FIFO */
welt_c_fifo_pointer in ;
/* The name of the output file . */
char * file_name ;
};
#endif
#ifndef txt_img_write_h
#define txt_img_write_h
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
Overview : Each firing of this actor consumes a pointer to a 2D vector
representation of an image , and writes the image to a file in a text - based
image format . The image file format that is utilized is the Simple Text - based
Image Format ( STIF ) supported in Welter .
Input (s ): The token type of the input is : pointer to a 2d vector
of integers .
Parameter (s ): The file_name parameter gives the name of file to be written
to .
Actor modes and transitions : The WRITE mode reads an image from the input
FIFO , and writes the image to the file that is specifed by the file_name
parameter . The INACTIVE mode does nothing .
Dataflow table :
Mode in_fifo
----------------------------
WRITE -1
INACTIVE 0
Initial mode : The actor starts out in the WRITE mode .
****************************************************************************** */
extern " C" {
#include "welt_c_util.h"
#include "welt_c_fifo.h"
}
#include "welt_cpp_actor.h"
#include "welt_cpp_graph.h"
#define TXT_IMG_WRITE_MODE_WRITE 1
#define TXT_IMG_WRITE_MODE_INACTIVE 2

class txt_img_write : public welt_cpp_actor {
public:
/* ************************************************************************
Construct a new txt_img_write actor that is associated with the given
file name , and connect the new actor to the given input FIFO .
************************************************************************ */
txt_img_write ( welt_c_fifo_pointer in_fifo , char * file_name ,
int index );
~ txt_img_write () override ;
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
/* Input FIFO */
welt_c_fifo_pointer in ;
/* The name of the output file . */
char * file_name ;
};
#endif