/*
libdmtx - Data Matrix Encoding/Decoding Library
Copyright (C) 2007 Mike Laughton

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Contact: mike@dragonflylogic.com
*/

/* $Id$ */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dmtx.h>

int
main(int argc, char *argv[])
{
   unsigned char testString[] = "30Q324343430794<OQQ";
   DmtxEncode encode;
   DmtxImage *image;
   DmtxDecode decode;
   DmtxRegion region;
   DmtxMessage *message;
   DmtxPixelLoc p0, p1;

   fprintf(stdout, "input:  \"%s\"\n", testString);

   /* 1) Encode a new Data Matrix barcode image and keep in memory */

   encode = dmtxEncodeStructInit();
   dmtxEncodeDataMatrix(&encode, strlen((char *)testString), testString,
         DMTX_SYMBOL_SQUARE_AUTO);

   /* 2) Take a copy of the new image before freeing DmtxEncode struct */

   image = dmtxImageMalloc(encode.image->width, encode.image->height);
   memcpy(image->pxl, encode.image->pxl, image->width * image->height *
         sizeof(DmtxRgb));

   dmtxEncodeStructDeInit(&encode);

   /* 3) Read back the Data Matrix barcode that was created above */

   p0.X = p0.Y = 0;
   p1.X = image->width - 1;
   p1.Y = image->height - 1;

   decode = dmtxDecodeStructInit(image, p0, p1, 5);

   region = dmtxDecodeFindNextRegion(&decode, NULL);
   if(region.found == DMTX_REGION_EOF)
      exit(0);

   message = dmtxDecodeMatrixRegion(&decode, &region, 1);
   if(message != NULL) {
      fputs("output: \"", stdout);
      fwrite(message->output, sizeof(unsigned char), message->outputIdx, stdout);
      fputs("\"\n\n", stdout);
      dmtxMessageFree(&message);
   }

   dmtxDecodeStructDeInit(&decode);
   dmtxImageFree(&image);

   exit(0);
}
