/*
*  ContourFinder.cpp
*
*
*  Created on 2/2/09.
*  Adapted from openframeworks ofxCvContourFinder
*
*/

#include "ContourFinder.h"

//--------------------------------------------------------------------------------
ContourFinder::ContourFinder()
{
	myMoments = (CvMoments*)malloc( sizeof(CvMoments) );
	reset();
}

//--------------------------------------------------------------------------------
ContourFinder::~ContourFinder()
{
	free( myMoments );
}

//--------------------------------------------------------------------------------
void ContourFinder::reset()
{
    blobs.clear();
    nBlobs = 0;

	objects.clear();
	nObjects = 0;
}

//--------------------------------------------------------------------------------
void ContourFinder::setTemplateUtils(TemplateUtils* _templates)
{
	templates=_templates;
}


/*int ContourFinder::findContours( ofxCvGrayscaleImage&  input,
									  int minArea,
									  int maxArea,
									  int nConsidered,
									  bool bFindHoles,
                                      bool bUseApproximation)
*/
//--------------------------------------------------------------------------------


int ContourFinder::findContours( ofxCvGrayscaleImage&  input,
									  int minArea,
									  int maxArea,
									  int nConsidered,

									  bool bFindHoles,
                                      bool bUseApproximation)
{
	reset();

	// opencv will clober the image it detects contours on, so we want to
    // copy it into a copy before we detect contours.  That copy is allocated
    // if necessary (necessary = (a) not allocated or (b) wrong size)
	// so be careful if you pass in different sized images to "findContours"
	// there is a performance penalty, but we think there is not a memory leak
    // to worry about better to create mutiple contour finders for different
    // sizes, ie, if you are finding contours in a 640x480 image but also a
    // 320x240 image better to make two ContourFinder objects then to use
    // one, because you will get penalized less.

	if( inputCopy.width == 0 )
	{
		inputCopy.allocate( input.width, input.height );
		inputCopy = input;
	}
	else
	{
		if( inputCopy.width == input.width && inputCopy.height == input.height )
			inputCopy = input;
		else
		{
			// we are allocated, but to the wrong size --
			// been checked for memory leaks, but a warning:
			// be careful if you call this funon with alot of different
			// sized "input" images!, it does allocation every time
			// a new size is passed in....
			//inputCopy.clear();
			inputCopy.allocate( input.width, input.height );
			inputCopy = input;
		}
	}

	CvSeq* contour_list = NULL;
	contour_storage = cvCreateMemStorage( 1000 );
	storage	= cvCreateMemStorage( 1000 );

	CvContourRetrievalMode  retrieve_mode
        = (bFindHoles) ? CV_RETR_LIST : CV_RETR_EXTERNAL;

        //en esta funcion llena el contourlist
	cvFindContours( inputCopy.getCvImage(), contour_storage, &contour_list,
                    sizeof(CvContour), retrieve_mode, bUseApproximation ? CV_CHAIN_APPROX_SIMPLE : CV_CHAIN_APPROX_NONE );
	CvSeq* contour_ptr= contour_list;



	nCvSeqsFound = 0;


	//BGUAMANMinArea
		CvMemStorage* storage2;
	storage2 = cvCreateMemStorage( 1000 );
	//-------------------------------------

	// put the contours from the linked list, into an array for sorting
	while( (contour_ptr != NULL) )
	{
		CvBox2D box=cvMinAreaRect2(contour_ptr);
		int objectId; // If the contour is an object, then objectId is its ID
		objectId=(bTrackObjects)? templates->getTemplateId(box.size.width,box.size.height): -1;



		if(objectId != -1 ) //If the blob is a object
		{
			Blob blob		= Blob();
			blob.id			= objectId;
			blob.isObject	= true;
			float area = cvContourArea( contour_ptr, CV_WHOLE_SEQ );

			cvMoments( contour_ptr, myMoments );

			// this is if using non-angle bounding box
			CvRect rect	= cvBoundingRect( contour_ptr, 0 );
			blob.boundingRect.x      = rect.x;
			blob.boundingRect.y      = rect.y;
			blob.boundingRect.width  = rect.width;
			blob.boundingRect.height = rect.height;

			//For anglebounding rectangle
			blob.angleBoundingBox=box;
			blob.angleBoundingRect.x	  = box.center.x;
			blob.angleBoundingRect.y	  = box.center.y;
			blob.angleBoundingRect.width  = box.size.height;
			blob.angleBoundingRect.height = box.size.width;
			blob.angle = box.angle;

			//TEMPORARY INITIALIZATION TO 0, Will be calculating afterwards.This is to prevent sending wrong data
			blob.D.x = 0;
			blob.D.y = 0;
			blob.maccel = 0;

			// assign other parameters
			blob.area                = fabs(area);
			blob.hole                = area < 0 ? true : false;
			blob.length 			 = cvArcLength(contour_ptr);

			blob.centroid.x			 = (myMoments->m10 / myMoments->m00);
			blob.centroid.y 		 = (myMoments->m01 / myMoments->m00);
			blob.lastCentroid.x 	 = 0;
			blob.lastCentroid.y 	 = 0;

			// get the points for the blob:
			CvPoint           pt;
			CvSeqReader       reader;
			cvStartReadSeq( contour_ptr, &reader, 0 );

    		for( int j=0; j < contour_ptr->total; j++ )
			{
				CV_READ_SEQ_ELEM( pt, reader );
				blob.pts.push_back( ofPoint((float)pt.x, (float)pt.y) );
			}
			blob.nPts = blob.pts.size();

			objects.push_back(blob);
		}
		else if(bTrackFingers && bRoundedPoints)


		{




		    //BGUAMAN
    //-------------------------------------------------
    	//-- simplify contour.


	CvSeq *contour_ptr_reduced= 0;



	contour_ptr_reduced = cvApproxPoly
	(
		contour_ptr,
		sizeof( CvContour ),
		storage2,
		CV_POLY_APPROX_DP,
//		cvContourPerimeter( &contour ) * 0.004,
		cvContourPerimeter( contour_ptr ) * valorconstante,
		0
	);
//--------------------------------------------------------






		       //cout<<"TROUNDED"<<endl;
		    //fabs para obtaener el valor absoluto del area de contorno
			float area = fabs( cvContourArea(contour_ptr, CV_WHOLE_SEQ) );
			if( (area > minArea) && (area < maxArea) )
			{    //descomentar luego
				//Blob blob=Blob();
				//float area = cvContourArea( contour_ptr, CV_WHOLE_SEQ );

				//CTI-ESPOL-BG
				//COmentario : aqui llena la secuencia de momentos myMoments con la funcion cvMoments
				cvMoments( contour_ptr, myMoments );

    //definimos una variable que decide de acuerdo al numero de  cada cuanto vamos a hacer un blob
                int eachpoint=floor(double(contour_ptr->total)/5);
                  //  cout<<"numero de puntos"<< contour_ptr_reduced->total<<endl;
                   int sumador =0;

				for(int m=0;m<contour_ptr_reduced->total;m++){

				    Blob blob=Blob();



				CvPoint p1 = *(CvPoint*)cvGetSeqElem(contour_ptr_reduced,m);
			//BGUAMAN
			//	sumador=sumador+eachpoint;



                //printf("x=%d y=%d",p1.x,p1.y);
                //primera parte copiada
               // printf("tamanio de la secuencia = %d",contour_ptr->total);
                	CvRect rect	= cvBoundingRect( contour_ptr, 0 );
				blob.boundingRect.x      = rect.x;
				blob.boundingRect.y      = rect.y;
				blob.boundingRect.width  = rect.width;
				blob.boundingRect.height = rect.height;
                //segunda parte copiada
                //hacemos un nuevo box cuando es por el metodo de fingers
                CvBox2D boxfinger=cvMinAreaRect2(contour_ptr);
                blob.angleBoundingRect.x	  = boxfinger.center.x;
				blob.angleBoundingRect.y	  = boxfinger.center.y;
				blob.angleBoundingRect.width  = boxfinger.size.height;
				blob.angleBoundingRect.height = boxfinger.size.width;
				blob.angle = boxfinger.angle;
                //tercera parte copiada
                //sacamos una nueva area
                //float nueva_area = fabs( cvContourArea(contour_ptr, CV_WHOLE_SEQ) );
                blob.area                = fabs(5.0f);
				blob.hole                = area < 0 ? true : false;
				blob.length 			 = cvArcLength(contour_ptr);
				//cuarta parte copiada
				blob.centroid.x			 = p1.x;
				blob.centroid.y 		 = p1.y;

				blob.lastCentroid.x 	 = 0;
				blob.lastCentroid.y 	 = 0;

				// cout<<"ID BLOBC:"<<blob.id<<"("<<blob.centroid.x<<","<<blob.centroid.y<<")"<<endl;


				blob.nPts = 0;

				blobs.push_back(blob);

				}

			}
		}

    else if(bTrackFingers && bCenterPoint)
		{






		  //  cout<<"TCENTERED"<<endl;
		    	    //fabs para obtaener el valor absoluto del area de contorno
			float area = fabs( cvContourArea(contour_ptr, CV_WHOLE_SEQ) );
			if( (area > minArea) && (area < maxArea) )
			{    //descomentar luego
				//Blob blob=Blob();
				//float area = cvContourArea( contour_ptr, CV_WHOLE_SEQ );

				//CTI-ESPOL-BG
				//COmentario : aqui llena la secuencia de momentos myMoments con la funcion cvMoments
				cvMoments( contour_ptr, myMoments );
				//CTI-ESPOL-BG
				//recorremos la lista para ver los punto de su contorno
                //
                CvMemStorage* almacenamiento_de_pts = cvCreateMemStorage(0);
    CvSeq* nuevo_contour = cvCreateSeq( 0, /* HAGA UNA SECUENCIA GENERICA */
    sizeof(CvSeq), /* SIEMPRE VA ACÁ ??? */
    sizeof(CvContour), /* TAMAÑO DE LA SECUENCIA */
    almacenamiento_de_pts /* EL CvMemStorage DE ALMACENAMIENTO */ );
    /*añadir blobs*/
    //definimos una variable que decide de acuerdo al numero de  cada cuanto vamos a hacer un blob
                Blob blobc=Blob();
                CvRect rect	= cvBoundingRect( contour_ptr, 0 );
				blobc.boundingRect.x      = rect.x;
				blobc.boundingRect.y      = rect.y;
				blobc.boundingRect.width  = rect.width;
				blobc.boundingRect.height = rect.height;

				//Angle Bounding rectangle
				blobc.angleBoundingRect.x	  = box.center.x;
				blobc.angleBoundingRect.y	  = box.center.y;
				blobc.angleBoundingRect.width  = box.size.height;
				blobc.angleBoundingRect.height = box.size.width;
				blobc.angle = box.angle;

				// assign other parameters
				blobc.area                = fabs(area);
				blobc.hole                = area < 0 ? true : false;
				blobc.length 			 = cvArcLength(contour_ptr);
				// AlexP
				// The cast to int causes errors in tracking since centroids are calculated in
				// floats and they migh land between integer pixel values (which is what we really want)
				// This not only makes tracking more accurate but also more fluid
				blobc.centroid.x			 = (myMoments->m10 / myMoments->m00);
				blobc.centroid.y 		 = (myMoments->m01 / myMoments->m00);
				blobc.lastCentroid.x 	 = 0;
				blobc.lastCentroid.y 	 = 0;

				// get the points for the blob:
				CvPoint           pt;
				CvSeqReader       reader;
				cvStartReadSeq( contour_ptr, &reader, 0 );

    			for( int j=0; j < min(TOUCH_MAX_CONTOUR_LENGTH, contour_ptr->total); j++ )
				{
					CV_READ_SEQ_ELEM( pt, reader );
					blobc.pts.push_back( ofPoint((float)pt.x, (float)pt.y) );
				}
				blobc.nPts = blobc.pts.size();



                int eachpoint=floor(double(contour_ptr->total)/5);

                   int sumador =0;
              // CvSeq* nuevo_contour;
				for(int m=0;m<5;m++){



				CvPoint p1 = *(CvPoint*)cvGetSeqElem(contour_ptr,sumador);
				sumador=sumador+eachpoint;

                blobc.puntos.push_back( ofPoint((float)p1.x, (float)p1.y) );
				}
                blobs.push_back(blobc);

			}

		}

		contour_ptr = contour_ptr->h_next;
	}

	nBlobs = blobs.size();
	nObjects = objects.size();
	// Free the storage memory.
	// Warning: do this inside this function otherwise a strange memory leak
	if( contour_storage != NULL ) { cvReleaseMemStorage(&contour_storage); }
	if( storage != NULL ) { cvReleaseMemStorage(&storage); }
	if( storage2 != NULL ) { cvReleaseMemStorage(&storage2); }
	/*
for(int y=0;y<nBlobs;y++){
    printf("Number of objects :  %d\n",nBlobs);
printf("el id es : %d",blobs[y].id);
}
*/

	return nBlobs;

}

