//
//   Author : Pei-Luan Tai
//   Contact: pt10f@my.fsu.edu
//   Last update: May 2, 2017
//***************************************/

#include "myDialog_lite.h"

#include <RVersion.h>
#include <TMath.h>
#include <TStyle.h>
#include <TGWindow.h>
#include <TGFrame.h>
#include <TBox.h>
#include <TF1.h>
#include <TH1.h>
#include <TLine.h>
#include <TPolyMarker.h>
#include <TText.h>
#include <TSpectrum.h>
#include <TCanvas.h>
#include <TRootEmbeddedCanvas.h>
#include <TGTextViewStream.h>
#include <TRandom.h>


////////////////////////////////////////////////////////////////////

class ROOTSCOPE_lite : public TGMainFrame {

private:

    int     fCmp;                       /* rebin rate */
    int     fWhich_active_line;         /* to set which line is active */
    float   fXrange_pick1;              /* range1 set by mouse click*/
    float   fXrange_pick2;              /* range2 set by mouse click*/
    float   fGeneral_pick1;             /* multi-purpose */
    float   fGeneral_pick2;             /* multi-purpose */

    float   fBG_const;                  /* bg value, const term */
    float   fBG_linear;                 /* bg value, linear term */


    TH1*    histo;                      /* the 1D histo */
    TH1*    histo_backup;               /* the 1D histo backup*/


    TF1*    simple_gaus;                /* only for testing*/
    TF1*    fTF1_bg_const;              /* bg = const */
    TF1*    fTF1_bg_linear;             /* bg = const + linear*x */
    TF1*    fTF1_gaus_bg;               /* y = guas(x) + a*x + c */
    TF1*    fTF1_gaus_bg_2;
    TF1*    fTF1_double_gaus_bg;


    TLine*  myline1;                     /* line marker 1 */
    TLine*  myline2;                     /* line marker 2 */
    TLine*  general_line1;
    TLine*  general_line2;

    Int_t   fH1_fillstyle;              /* fill style */
    Int_t   fH1_linewidth;              /* line width */
    Int_t   fLogScale_style;


    TList*          fTexts;             /* text labels for found peaks*/
    Float_t         fTextSize;
    Float_t         fTextPrecision;
    TString         fPrecisionFormat;
    TPolyMarker*    fPolyMarkers;       /* markers for found peaks*/
    TSpectrum*      peakFinder;
    Float_t         fSigma_guess;
    Float_t         fThreshold;


    TRootEmbeddedCanvas *emb_Canvas;     /* Canvas container */
    TCanvas             *c1;             /* Canvas  */
    TGTextViewostream   *fText_viewer;   /* for show user message */


    void SetMarker(Event_t*);

    void SetMarker2(Event_t*);

    void SetGeneralMarker( Event_t*);

    void Set_Y_Range();

    void Expand_dlg();

    void Expand();

    void Unexpand();

    void Go_half_left();

    void Go_half_right();

    void Clear_Marker();


    void Fit_Gaussian();

    void Fit_Double_Gaussian( );

    void Fit_Background();       // for bg = const

    void Fit_Background_pol1();  // for bg = const + linear * x

    void Set_Background();

    void Get_Sum();


    void Rebin_compress();

    void Rebin_uncompress();


    void Set_histo_title();


    float Get_Max_range ( float, float);

    float Get_Min_range ( float, float);

    void SetActiveLine( TLine* , float, float );

    void Switch_ActiveLine(TLine*, TLine*);

    void Set_Line( TLine* l, float x1, float y1, float x2, float y2 );

    void Set_Xrange_pick( float );


    void Set_init_fit_value( double, double, double& , double& , double& );

    void To_find_peaks();

    void To_show_find_peaks_dlg();

    void To_Draw_bg();

    void To_Update_Canvas();

    void To_Change_histo_x_range();

    void To_change_linewidth();

    void To_change_fillstyle();

    void To_set_logscale();

    void To_writeoutFile();

    void To_readinFile();


public:



    // constuctor: widgets are placed here.
    ROOTSCOPE_lite(  const TGWindow * p,  TH1* );  /* base windows, histogram_input  */



    //----------------------------------------- for key response
    void To_response(Event_t*);



    void SetOneDHisto( TH1* h_input );

    TH1* GetOneDHisto();

    TString GetMessage(){ return fText_viewer->GetText()->AsString();  }

    TCanvas* GetCanvas() { return c1; }

};




void ROOTSCOPE_lite::To_response(Event_t* e) {

    // the center for key and mouse reponse.

    // Note: Event_t data type see:
    // https://root.cern.ch/root/html/Event_t.html
    // for mouse click : check EEventType

    // Note: for key symbols
    // https://root.cern.ch/doc/v608/KeySymbols_8h.html
    // https://root.cern.ch/doc/v606/GuiTypes_8h.html


    // prevent to drag and move the objects on the canvas.
    gPad->FindObject("TFrame")->SetBit(TBox::kCannotMove);

    // my own lookup table.
    const unsigned key_arrow_left  = 113;
    const unsigned key_arrow_right = 114;


    bool isCtrl = false;
    bool isAlt  = false;
    if ( e->fState & kKeyMod1Mask ){isAlt=true; }  else { isAlt = false;   }
    if ( e->fState & kKeyControlMask ) { isCtrl=true; } else { isCtrl = false;  }


    // using LookuString to get the key mapping.
    UInt_t key_symbol;
    char tmp[2];
    if ( e->fType == kGKeyPress ) {
        gVirtualX->LookupString( e, tmp,sizeof(tmp), key_symbol );
    }

    if ( e->fType == kButtonPress && e->fCode == kButton1Down && !isCtrl && !isAlt )
    { SetMarker( e ); } // right click to set a marker.

    // ctl + right click.
    if ( e->fType == kButtonPress && e->fCode == kButton1Down && isCtrl && !isAlt  )
    { SetGeneralMarker( e ); }




    //_______key
    if ( e->fType == kGKeyPress && !isCtrl && !isAlt   ) {


        if( key_symbol == kKey_m  ) { SetMarker( e ); }

        else if( key_symbol == kKey_n )  { Clear_Marker(); }

        else if( key_symbol == kKey_g )  { Fit_Gaussian( ); }

        else if( key_symbol == kKey_G )  { Fit_Double_Gaussian( ); }

        else if( key_symbol == kKey_y )  { Set_Y_Range( ); }




        else if ( key_symbol == kKey_b  ) { Fit_Background(); }  // bg = const.

        else if ( key_symbol == kKey_1 )  { Fit_Background_pol1(); } // bg = const + linear* x.

        else if ( key_symbol == kKey_s )  { Get_Sum( ); }

        else if( key_symbol == kKey_E )  { Expand_dlg(); }

        else if( key_symbol == kKey_e )  { Expand( ); }

        else if( key_symbol == kKey_o )  { Unexpand( ); }

        else if( e->fCode == key_arrow_left )  { Go_half_left( ); }

        else if( e->fCode == key_arrow_right )  { Go_half_right( ); }

        else if( key_symbol == kKey_x ) { Rebin_compress(); }

        else if( key_symbol == kKey_X ) { Rebin_uncompress(); }

        else if (key_symbol == kKey_F1 ) { To_change_fillstyle(); }

        else if (key_symbol == kKey_F2 ) { To_change_linewidth(); }

        else if( key_symbol == kKey_F3  ) { To_set_logscale(); }

        else if( key_symbol == kKey_f  ) { To_find_peaks(); }




    }



    //_______alt + key
    if ( e->fType == kGKeyPress && !isCtrl && isAlt   ) {

        // alt+m
        if ( key_symbol == kKey_m  ) { SetMarker2( e ); } //to open a dlg, let user to set value.

        // alt+b
        else if ( key_symbol == kKey_b ) { Set_Background(); } // open a dlg, let user to set value.

        // alt+r
        else if(  key_symbol == kKey_r ) { To_Change_histo_x_range(); }

    }


    //_______ctrl + key
    if ( e->fType == kGKeyPress && isCtrl && !isAlt   ) {

        // ctl + t
        if( key_symbol == kKey_t ) { Set_histo_title(); }

         // ctl + w
        else if( key_symbol == kKey_w  ) { To_writeoutFile(); }

        // ctl + f
        else if( key_symbol == kKey_f  ) { To_show_find_peaks_dlg(); }

    }

}



void ROOTSCOPE_lite::SetMarker(Event_t* e){

    //------------------------------------------------------  -| get x_pos, y_pos.
    int   px = e->fX;
    float xx = gPad->AbsPixeltoX(px);
    int binx = histo->GetXaxis()-> FindBin(xx);

    float x_pos = histo-> GetXaxis()-> GetBinCenter( binx );
    float y_pos =  c1->GetUymax();

    //------------------------------------------------| set x_pos, y_pos to active line.
    Switch_ActiveLine( myline1, myline2 ); //switch between fWhich_actice_line.
    if ( fWhich_active_line == 1 )  { SetActiveLine( myline1, x_pos, y_pos );  }
    else if ( fWhich_active_line  == 2  ) { SetActiveLine( myline2, x_pos, y_pos );}

    Set_Xrange_pick( x_pos );


    // show message
    float y_counts = histo-> GetBinContent( binx );
    *fText_viewer << Form("x = %6.3f\t\t(bin \t= %d), y = %6.3f ", x_pos, binx, y_counts)  <<  endl;
    fText_viewer->ShowBottom();


}



void ROOTSCOPE_lite::SetMarker2(Event_t* e){

    // we will pop up a dialog for user to set the marker

    //------------------------------------------------------------| get x_pos, y_pos.
    float x_pos;
    float x_pos_limit1 =  histo->GetXaxis()->GetXmin();
    float x_pos_limit2 =  histo->GetXaxis()->GetXmax();

    // open a dialog for user to input marker position ( x_pos )
    new Dlg_Set_Marker( gClient->GetRoot(), this,
                        10, 10,
                        &x_pos, x_pos_limit1, x_pos_limit2 );

    float y_pos =  c1->GetUymax();




    // check user's input is within the boundaries.
    if( x_pos < x_pos_limit1 || x_pos > x_pos_limit2 ) {

        // when outside the boundaires.
        *fText_viewer << "ERROR marker is out of the bounaries" <<  endl;
        fText_viewer->ShowBottom();

    }
    else {

        // when within the boundaires.
        Switch_ActiveLine( myline1, myline2 ); //switch between fWhich_actice_line.
        if ( fWhich_active_line == 1 )        { SetActiveLine( myline1, x_pos, y_pos );  }
        else if ( fWhich_active_line  == 2  ) { SetActiveLine( myline2, x_pos, y_pos );  }

        Set_Xrange_pick( x_pos );

        // show message
        int binx = histo->GetXaxis()-> FindBin( x_pos );
        *fText_viewer << Form("x = %6.3f\t\t(bin \t= %d) ", x_pos, binx)  <<  endl;
        fText_viewer->ShowBottom();

    }

}



void ROOTSCOPE_lite::SetGeneralMarker( Event_t* e) {



    int   px = e->fX;
    float xx = gPad->AbsPixeltoX(px);
    int binx = histo->GetXaxis()-> FindBin(xx);

    float x_pos = histo-> GetXaxis()-> GetBinCenter( binx );
    float y_pos1 =  gPad->GetUymin();
    float y_pos2 =  gPad->GetUymax();

    if  ( x_pos != fGeneral_pick1 )
    {
        fGeneral_pick2 = fGeneral_pick1;
        fGeneral_pick1 = x_pos;
        c1->cd();

        Set_Line( general_line1, x_pos, y_pos1, x_pos, y_pos2 );
        Set_Line( general_line2, fGeneral_pick2, y_pos1, fGeneral_pick2, y_pos2 );
        general_line1->Draw();
        general_line2->Draw();
    }

    c1->Update();
}


void ROOTSCOPE_lite::Set_Y_Range( ) {

    if ( fLogScale_style != true ){

        float y_max;
        // passing by reference to get y_max from user input
        new Dlg_Set_YRange(gClient->GetRoot(), this,
                            10, 10,
                            &y_max , c1->GetUymax() );

        histo->GetYaxis()->SetRangeUser( 0, y_max );
        To_Update_Canvas();

    } else {

        *fText_viewer <<
        Form("In log scale mode, set y range will have an issue in display.")  <<  endl;
        fText_viewer->ShowBottom();
    }

}


void ROOTSCOPE_lite::Clear_Marker(){

    // hide the line makers, and clear the fitting lines.

    myline1->SetX1( 0 );
    myline1->SetX2( 0 );
    myline1->SetY1( 0 );
    myline1->SetY2( 0 );
    myline1->Draw();
    c1->Update();

    myline2->SetX1( 0 );
    myline2->SetX2( 0 );
    myline2->SetY1( 0 );
    myline2->SetY2( 0 );
    myline2->Draw();

    To_Update_Canvas();

}





void ROOTSCOPE_lite::Set_init_fit_value( double range1, double range2,
                                      double &h, double &c, double &sigma )
{



    int bin1 = histo -> GetXaxis()-> FindBin(range1);
    int bin2 = histo -> GetXaxis()-> FindBin(range2);

    //-------------------------------------------| get init height and center

    int     bin_max_y   = -99999;
    double  max_y       = -99999;


    for( int bin=bin1; bin<=bin2; bin++) {

        double y = histo -> GetBinContent( bin );

        if (y > max_y) {  max_y = y; bin_max_y = bin; }

    }

    c = histo-> GetXaxis()-> GetBinCenter( bin_max_y );

    h = max_y - fBG_const - fBG_linear*c;




    //-------------------------------------------| get init sigma

    // search for the x position where we have 1/2 * max_y

    int bin_left  = -1;
    int bin_right = -1;


    // --------------------------------------------- searching direction: from max to left
    for( int bin=bin_max_y; bin<=bin1; bin--)
    {

        double y = histo -> GetBinContent( bin );

        if ( y < max_y/2.0 ) {  bin_left = bin; break; }

    }

    // --------------------------------------------- searching direction: from max to right
    for( int bin=bin_max_y; bin<=bin2; bin++)
    {

        double y = histo -> GetBinContent( bin );

        if ( y < max_y/2.0 ) {  bin_right = bin; break; }

    }



    // --------------------------------------------- to assign sigma
    if( bin_left != -1 && bin_right != -1 ) {

        sigma = histo-> GetXaxis()-> GetBinCenter( bin_right ) -
                histo-> GetXaxis()-> GetBinCenter( bin_left ) ;
    }
    else if ( bin_left != -1 && bin_right == -1 ) {

        // only have left part
        sigma = 2*( c - histo-> GetXaxis()-> GetBinCenter( bin_left ) );
    }
    else if ( bin_left == -1 && bin_right != -1 ) {

        // only have right part
        sigma = 2*( histo-> GetXaxis()-> GetBinCenter( bin_right ) - c   );
    }
    else {

        sigma = range2 - range1 ;
    }



}



void ROOTSCOPE_lite::Fit_Gaussian() {


     // organize the range
    float xMax = Get_Max_range( fXrange_pick1, fXrange_pick2 );
    float xMin = Get_Min_range( fXrange_pick1, fXrange_pick2 );

    // to ensure we have a proper range.
    if( xMax != xMin )
    {
        Clear_Marker();

        double init_h, init_c, init_sigma;
        Set_init_fit_value( xMin, xMax, init_h, init_c, init_sigma );
        // passing reference to get init_h, init_c, and init_sigma.


        fTF1_gaus_bg->SetParameters( fBG_const, fBG_linear, init_h, init_c, init_sigma );


         fTF1_gaus_bg->FixParameter 	( 0, fBG_const  );
        fTF1_gaus_bg->FixParameter 	( 1, fBG_linear );
        fTF1_gaus_bg->SetParLimits  ( 2, init_h*0.50, init_h*1.50);
        fTF1_gaus_bg->SetParLimits  ( 3, xMin, xMax ); // center
        fTF1_gaus_bg->SetParLimits  ( 4, init_sigma*0.01, init_sigma*1.5 );

        histo->Fit( fTF1_gaus_bg, "MQNB", "", xMin, xMax  ); // B = to set boundary.


        // to extend the plot range from two markers to two absolute limits.
        float lower_limit = histo->GetXaxis()->GetXmin();
        float upper_limit = histo->GetXaxis()->GetXmax();
        fTF1_gaus_bg->SetRange( lower_limit, upper_limit );
        fTF1_gaus_bg->SetLineColor(kRed);
        fTF1_gaus_bg->SetLineWidth(2);
        fTF1_gaus_bg->SetLineStyle(1);
        fTF1_gaus_bg->Draw("same");

        c1->Update();


        // -----------------------------------------------| show message
        float fitted_h = fTF1_gaus_bg->GetParameter(2);
        float fitted_c = fTF1_gaus_bg->GetParameter(3);
        float fitted_sigma = fTF1_gaus_bg->GetParameter(4);

        float area = TMath::Sqrt( 2 * TMath::Pi() ) * fitted_h * fitted_sigma;
        float FWHM = 2.35482 * fitted_sigma;
        float chisqr = -0.0;
        if( fTF1_gaus_bg->GetNDF() != 0 ) {
            chisqr = fTF1_gaus_bg->GetChisquare()/fTF1_gaus_bg->GetNDF();
        }



        *fText_viewer <<
            Form( "\nCenter = %5.3f, Area/cmp = %5.2f, FWHM = %5.2f, Chisqr/N = %5.2f (cmp = %d)",
            fitted_c, area/fCmp, FWHM, chisqr, fCmp ) <<  endl;
        fText_viewer->ShowBottom();

    }

}




void ROOTSCOPE_lite::Fit_Double_Gaussian() {

    // organize the range
    float xMax = Get_Max_range( fXrange_pick1, fXrange_pick2 );
    float xMin = Get_Min_range( fXrange_pick1, fXrange_pick2 );

    Bool_t isNoValues = false;
    float init_h1=0, init_c1=0, init_sigma1=0;
    float init_h2=0, init_c2=0, init_sigma2=0;



    // to ensure we have a proper range.
    if( xMax != xMin )
    {
        // use ctl+mouse right click to set up
        init_c1 =  fGeneral_pick1;
        init_c2 =  fGeneral_pick2;
        init_h1 = histo-> GetBinContent( histo->FindBin( fGeneral_pick1 ) );
        init_h2 = histo-> GetBinContent( histo->FindBin( fGeneral_pick2 ) );

        Dlg_double_gaussian* get_initial_valuse
        = new Dlg_double_gaussian(  gClient->GetRoot(),
                                    &init_h1,
                                    &init_c1,
                                    &init_sigma1,
                                    &init_h2,
                                    &init_c2,
                                    &init_sigma2);

        if( init_h1==0 && init_c1==0 && init_sigma1==0 &&
            init_h2==0 && init_c2==0 && init_sigma2==0) { isNoValues = true; }
        // it happens when we press "cancel" btn.
    }


    if( !isNoValues && (xMax != xMin) ){

        Clear_Marker();

        fTF1_double_gaus_bg
        ->SetParameters(    fBG_const, fBG_linear,
                            init_h1, init_c1, init_sigma1,
                            init_h2, init_c2, init_sigma2 );

        init_sigma1 = init_sigma1/2.35482;
        init_sigma2 = init_sigma2/2.35482;

        fTF1_double_gaus_bg->FixParameter ( 0, fBG_const  );
        fTF1_double_gaus_bg->FixParameter ( 1, fBG_linear );
        fTF1_double_gaus_bg->SetParLimits ( 2, 0, init_h1*2);
        fTF1_double_gaus_bg->SetParLimits ( 3, xMin, xMax ); // center
        fTF1_double_gaus_bg->SetParLimits ( 4, init_sigma1*0.01, init_sigma1*3 );
        fTF1_double_gaus_bg->SetParLimits ( 5, 0, init_h1*2);
        fTF1_double_gaus_bg->SetParLimits ( 6, xMin, xMax ); // center
        fTF1_double_gaus_bg->SetParLimits ( 7, init_sigma2*0.01, init_sigma2*3 );


        histo->Fit( fTF1_double_gaus_bg, "MQNB", "", xMin, xMax  );

        float lower_limit = histo->GetXaxis()->GetXmin();
        float upper_limit = histo->GetXaxis()->GetXmax();
        fTF1_double_gaus_bg->SetRange( lower_limit, upper_limit );
        fTF1_double_gaus_bg->SetLineWidth(3);
        fTF1_double_gaus_bg->Draw("same");

        float fitted_h1 = fTF1_double_gaus_bg->GetParameter(2);
        float fitted_c1 = fTF1_double_gaus_bg->GetParameter(3);
        float fitted_sigma1 = fTF1_double_gaus_bg->GetParameter(4);

        float fitted_h2 = fTF1_double_gaus_bg->GetParameter(5);
        float fitted_c2 = fTF1_double_gaus_bg->GetParameter(6);
        float fitted_sigma2 = fTF1_double_gaus_bg->GetParameter(7);



        fTF1_gaus_bg
        ->SetParameters( fBG_const, fBG_linear, fitted_h1, fitted_c1, fitted_sigma1 );
        fTF1_gaus_bg->SetRange( lower_limit, upper_limit );
        fTF1_gaus_bg->SetLineColor(kGreen);
        fTF1_gaus_bg->SetLineWidth(2);
        fTF1_gaus_bg->SetLineStyle(2);
        fTF1_gaus_bg->Draw("same");

        fTF1_gaus_bg_2
        ->SetParameters( fBG_const, fBG_linear, fitted_h2, fitted_c2, fitted_sigma2 );
        fTF1_gaus_bg_2->SetRange( lower_limit, upper_limit );
        fTF1_gaus_bg_2->SetLineColor(kGreen);
        fTF1_gaus_bg_2->SetLineWidth(2);
        fTF1_gaus_bg_2->SetLineStyle(2);
        fTF1_gaus_bg_2->Draw("same");

        c1->Update();


        // -----------------------------------------------| show message

        float area1 = TMath::Sqrt( 2 * TMath::Pi() ) * fitted_h1 * fitted_sigma1;
        float FWHM1 = 2.35482 * fitted_sigma1;

        float area2 = TMath::Sqrt( 2 * TMath::Pi() ) * fitted_h2 * fitted_sigma2;
        float FWHM2 = 2.35482 * fitted_sigma2;


        float chisqr = -0.0;
         if( fTF1_double_gaus_bg->GetNDF() != 0 ) {
            chisqr = fTF1_double_gaus_bg->GetChisquare()/fTF1_double_gaus_bg->GetNDF();
        }



        *fText_viewer
        <<
        Form( "\nPeak1 Center = %5.3f, Area/cmp = %5.2f, FWHM = %5.2f\n",
            fitted_c1, area1/fCmp, FWHM1 )
        <<
        Form( "Peak2 Center = %5.3f, Area/cmp = %5.2f, FWHM = %5.2f\n",
            fitted_c2, area2/fCmp, FWHM2 )
        <<
        Form( "Chisqr/N = %5.2f (cmp=%d)", chisqr, fCmp )
        <<  endl;
        fText_viewer->ShowBottom();

    }

}


void ROOTSCOPE_lite::Fit_Background_pol1() {


     // organize the range
    float xMax = Get_Max_range( fXrange_pick1, fXrange_pick2 );
    float xMin = Get_Min_range( fXrange_pick1, fXrange_pick2 );

    // to ensure we have a proper range.
    if( xMax != xMin )
    {
         // doing fitting
        histo->Fit( fTF1_bg_linear, "MNQ", "", xMin, xMax);
        // N option is to not display
        // (important here, otherwise it will create new TGraph....)

        fBG_const  = fTF1_bg_linear->GetParameter(0);
        fBG_linear = fTF1_bg_linear->GetParameter(1);

        To_Draw_bg();

    }

}



void ROOTSCOPE_lite::Fit_Background() {

    fBG_linear = 0.0 ; // reset it, since we only use fBG_const

     // organize the range
    float xMax = Get_Max_range( fXrange_pick1, fXrange_pick2 );
    float xMin = Get_Min_range( fXrange_pick1, fXrange_pick2 );

    // to ensure we have a proper range.
    if( xMax != xMin )
    {
         // doing fitting
        histo->Fit( fTF1_bg_const, "MNQ", "", xMin, xMax);
        // N option is to not display

        fBG_const = fTF1_bg_const->GetParameter(0);

        To_Draw_bg();



    }

}


void ROOTSCOPE_lite::Set_Background() {

    fBG_linear = 0; // reset

    // pop up a dialog for user to set bg value.
    new Dlg_Set_Background( gClient->GetRoot(), this, 10, 10, &fBG_const );

    fTF1_bg_const->SetParameter(0, fBG_const); // set the 0th par = fBG_const

    To_Draw_bg();

}



void ROOTSCOPE_lite::Get_Sum() {

     // organize the range
    float xMax = Get_Max_range( fXrange_pick1, fXrange_pick2 );
    float xMin = Get_Min_range( fXrange_pick1, fXrange_pick2 );

    int binx1 = histo->GetXaxis()-> FindBin(xMin);
    int binx2 = histo->GetXaxis()-> FindBin(xMax);

    float area = 0;
    int     counts = 0;
    float dX   =histo->GetXaxis()->GetBinWidth(1);
    for( Int_t i =binx1; i<=binx2; i++ ){

         float height = histo-> GetBinContent( i );
         area += ( height * dX );

         float bgValue
         = fBG_const + fBG_linear * histo->GetXaxis()->GetBinCenter(i);

        if( height > bgValue) { counts += height; }
    }

    float y1 = fBG_const + fBG_linear * xMin;
    float y2 = fBG_const + fBG_linear * xMax;
    float  w = xMax - xMin;

    float area_bg = 0.5 * w * ( y1 + y2 );

    area = area - area_bg;


    *fText_viewer
    << Form("\nsum over the range %.1f to %.1f (binwith = %.2f total bin = %d)\n" , xMin, xMax, histo->GetBinWidth(1), histo->GetNbinsX() )
    << Form("area/cmp = %.1f (bg = %.f, cmp = %d). \n",  area/fCmp, area_bg, fCmp)
    << Form("We have %d counts above bg.", counts)  <<  endl;
    fText_viewer->ShowBottom();

}

void ROOTSCOPE_lite::To_Draw_bg() {

    Clear_Marker();
    // absolute limits
    float lower_limit = histo->GetXaxis()->GetXmin();
    float upper_limit = histo->GetXaxis()->GetXmax();



    if( fBG_linear == 0 ){

        fTF1_bg_const->SetRange( lower_limit, upper_limit );
        fTF1_bg_const->Draw("same");
        *fText_viewer << Form( "\nbg= %6.2f", fBG_const ) <<  endl;
    }
    else {

        fTF1_bg_linear->SetRange( lower_limit, upper_limit );
        fTF1_bg_linear->Draw("same");
        *fText_viewer << Form( "\nbg= %6.3f + %6.3f * x", fBG_const, fBG_linear ) <<  endl;
    }



    c1->Update();


    fText_viewer->ShowBottom();

}




void ROOTSCOPE_lite::Expand_dlg() {

    /* pop up a dialog for user to input the range for expanding. */

    float xMax = Get_Max_range( fXrange_pick1, fXrange_pick2 );
    float xMin = Get_Min_range( fXrange_pick1, fXrange_pick2 );

    // two absolute limits.
    float lower_limit = histo->GetXaxis()->GetXmin();
    float upper_limit = histo->GetXaxis()->GetXmax();

    new Dlg_Set_XRange(gClient->GetRoot(), 10, 10,
                        lower_limit, upper_limit,
                         &xMin , &xMax );



    if( xMax != xMin )
    {

        histo->GetXaxis()->SetRangeUser( xMin, xMax);
        histo->Draw();
        c1->Update();

    }

    // update markers
    fXrange_pick1 = xMin;
    fXrange_pick2 = xMax;


}


void ROOTSCOPE_lite::Expand( ) {



     // organize the range
    float xMax = Get_Max_range( fXrange_pick1, fXrange_pick2 );
    float xMin = Get_Min_range( fXrange_pick1, fXrange_pick2 );

    // to ensure we have a proper range.
    if( xMax != xMin )
    {

        histo->GetXaxis()->SetRangeUser( xMin, xMax);
        To_Update_Canvas();

    }

}



void ROOTSCOPE_lite::Unexpand( ) {


    histo->GetXaxis()->SetRange();
    histo->GetYaxis()->UnZoom();
    To_Update_Canvas();

    fXrange_pick1 = histo->GetXaxis()->GetXmin();
    fXrange_pick2 = histo->GetXaxis()->GetXmax();


}




void ROOTSCOPE_lite::Rebin_compress() {

    // store the current view range
    int bin_local_1 = histo->GetXaxis()->GetFirst();
    int bin_local_2 = histo->GetXaxis()->GetLast();
    float x1 = histo->GetXaxis()->GetBinCenter( bin_local_1 );
    float x2 = histo->GetXaxis()->GetBinCenter( bin_local_2 );

    // reset
    histo_backup->Copy( *histo  );

    fCmp += 1;
    histo->Rebin( fCmp );
    histo->GetXaxis()->SetRangeUser( x1, x2 );
    To_Update_Canvas();


    *fText_viewer << Form( "\n rebin by %d", fCmp ) <<  endl;
    fText_viewer->ShowBottom();


}





void ROOTSCOPE_lite::Rebin_uncompress() {


     // store the current view range
    int bin_local_1 = histo->GetXaxis()->GetFirst();
    int bin_local_2 = histo->GetXaxis()->GetLast();
    float x1 = histo->GetXaxis()->GetBinCenter( bin_local_1 );
    float x2 = histo->GetXaxis()->GetBinCenter( bin_local_2 );

    // reset
    histo_backup->Copy( *histo  );


    if( fCmp > 1 ) {
        fCmp -= 1 ;
        histo->Rebin( fCmp );
    } else {
        fCmp = 1;
    }
    histo->GetXaxis()->SetRangeUser( x1, x2 );
    To_Update_Canvas();


    *fText_viewer << Form( "\n rebin by %d", fCmp ) <<  endl;
    fText_viewer->ShowBottom();

}




void ROOTSCOPE_lite::Go_half_left( ) {

    // this function can only work after we expend the histo


    // local
    int bin_local_1 = histo->GetXaxis()->GetFirst();
    int bin_local_2 = histo->GetXaxis()->GetLast();
    float xMin = histo->GetXaxis()->GetBinCenter( bin_local_1 );
    float xMax = histo->GetXaxis()->GetBinCenter( bin_local_2 );

    // get the absolute limits ( not local view )
    float lower_limit = histo->GetXaxis()->GetXmin();
    float upper_limit = histo->GetXaxis()->GetXmax();

    if( xMin == lower_limit && xMax == upper_limit ){

        // user don't expand yet,
        // so we should do noting.

    } else {

        float shift = ( xMax - xMin )/2.0 ;
        float next_xR;
        float next_xL;

        if ( (xMin - shift - lower_limit) < 0 ) {

            // we cannot move to left anymore
            next_xL = lower_limit;
            next_xR = xMax - (xMin-lower_limit);
        } else {

            // good to move to left
            next_xL = xMin - shift;
            next_xR = xMax - shift;
        }



        histo->GetXaxis()->SetRangeUser( next_xL, next_xR);


        // update the makers, important!!
        fXrange_pick1 = next_xL;
        fXrange_pick2 = next_xR;
        Clear_Marker();

        To_Update_Canvas();

    }




}



void ROOTSCOPE_lite::Go_half_right( ) {

    // this function only work after we expend the histo

     // local
    int bin_local_1 = histo->GetXaxis()->GetFirst();
    int bin_local_2 = histo->GetXaxis()->GetLast();
    float xMin = histo->GetXaxis()->GetBinCenter( bin_local_1 );
    float xMax = histo->GetXaxis()->GetBinCenter( bin_local_2 );

    // get the absolute limits ( not local view )
    float lower_limit = histo->GetXaxis()->GetXmin();
    float upper_limit = histo->GetXaxis()->GetXmax();

    if( xMin == lower_limit && xMax == upper_limit ){

        // user don't expand yet,
        // so we should do noting.

    } else {

        float shift = ( xMax - xMin )/2.0 ;
        float next_xR;
        float next_xL;

        if ( ( upper_limit  - (xMax + shift)  ) < 0 ) {

            // we cannot move to right anymore
            next_xL = xMin + (upper_limit-xMax);
            next_xR = upper_limit;
        } else {

            // good to move to left
            next_xL = xMin + shift;
            next_xR = xMax + shift;
        }


        histo->GetXaxis()->SetRangeUser( next_xL, next_xR);


         // update the makers, important!!
        fXrange_pick1 = next_xL;
        fXrange_pick2 = next_xR;
        Clear_Marker();

        To_Update_Canvas();


    }
}





float ROOTSCOPE_lite::Get_Max_range( float x1, float x2) {
    if ( x1 > x2 ) { return x1; }
    else { return x2; }
}


float ROOTSCOPE_lite::Get_Min_range( float x1, float x2) {
    if ( x1 < x2 ) { return x1; }
    else { return x2; }
}


void ROOTSCOPE_lite::Set_Xrange_pick( float x) {

    if  ( x != fXrange_pick1 )
    {
        fXrange_pick2 = fXrange_pick1;
        fXrange_pick1 = x;
    }
}


void ROOTSCOPE_lite::Switch_ActiveLine(TLine* l1, TLine* l2) {

    float l1_x = l1->GetX1();
    float l2_x = l2->GetX1();

    if ( l1_x != l2_x ) {

        if ( fWhich_active_line == 1 ) {

            l1->SetLineColor( kOrange );
            l2->SetLineColor( kBlack );
            fWhich_active_line = 2;

        }
        else if ( fWhich_active_line == 2) {
            l2->SetLineColor( kOrange );
            l1->SetLineColor( kBlack );
            fWhich_active_line = 1;

        }

    } else {
        l1->SetLineColor( kOrange );
        l2->SetLineColor( kBlack );
        fWhich_active_line = 2;
    }

}




void ROOTSCOPE_lite::Set_Line( TLine* l, float x1, float y1, float x2, float y2 ) {

    l->SetX1( x1 ); l->SetY1( y1 );
    l->SetX2( x2 ); l->SetY2( y2 );

}

void ROOTSCOPE_lite::SetActiveLine( TLine* l, float x_pos, float y_pos ) {

    Set_Line( l,x_pos, 0, x_pos, y_pos );
    l->Draw();
    c1->cd(); c1->Update();
}


void ROOTSCOPE_lite::Set_histo_title() {

    TString h_title = histo->GetTitle();


    // pop up a dialog for user to change histo title.
    new Dlg_Set_hTitle( gClient->GetRoot(), this, 10, 10,
                        &h_title );

    histo->SetTitle( h_title.Data() );
    histo_backup = (TH1*) histo->Clone();

    To_Update_Canvas();

}


void ROOTSCOPE_lite::To_Update_Canvas(){

    gStyle->SetOptStat(kFALSE);
    c1->cd();
    histo->Draw(); // this command will clean the fitted lines.
    c1->Update();

}



void ROOTSCOPE_lite::To_Change_histo_x_range() {

    /* pop up a dialog for user to input the range for changing histogram x range. */


    float xlow_select;
    float xup_select;

    // to reset the compress before we change the x range.
    histo_backup->Copy( *histo  );


    // to retrieve the setup from the original histo.
    float xlow = histo->GetXaxis()->GetXmin();
    float xup  = histo->GetXaxis()->GetXmax();
    int binx1  = histo->GetXaxis()-> FindBin(xlow);
    int binx2  = histo->GetXaxis()-> FindBin(xup);
    float binWidth   =histo->GetXaxis()->GetBinWidth(1);

    new Dlg_Change_histo_X_Range(gClient->GetRoot(), 10, 10,
                        xlow, xup,
                        &xlow_select , &xup_select );


    if( (xlow_select == xlow) && (xup_select == xup) )
    {
        /* do nothing, because we don't change x range.*/
        histo->Rebin( fCmp );
    }
    else{



        float   diff;
        float   interval;
        int     ratio;
        int     binN2;
        float   xup2;
        float   xlow2;

        //========================================== xup2 calculation
        interval = xup_select-xlow;
        ratio = interval/binWidth + 0.000005;
        // we have truncation (int) = (float)
        // about the precision testing
        // suppose xup = 40.0      --> exact 200 bins.
        // xup = 40.00001  -> 201 bins
        // xup = 40.000001 -> 200 bins


        diff = interval - ratio * binWidth ;
        // note: due to the truncation, interval alway > (ratio*binWidth)

        if ( abs(diff) < 0.00001 ) {
            xup2 = xup_select;
            binN2 = ratio; }
        else {
          binN2 = ratio + 1;
          xup2 = binN2 * binWidth + xlow; }



        //========================================== xlow2 calculation
        interval = xup2 - xlow_select;
        ratio = interval/binWidth + 0.000005;
        diff = interval - ratio*binWidth;
        // note: due to the truncation, interval alway > (ratio*binWidth)

        if ( diff < 0.00001 ) {
            xlow2 = xlow_select;
            binN2 = ratio; }
        else {
            binN2 = ratio + 1;
            xlow2 = xup2 - binN2 * binWidth;
        }


        TH1F* histoNew = new TH1F( "histoNew", "histoNew", binN2, xlow2, xup2 );
        histoNew->SetTitle( histo->GetTitle() );
        histoNew->SetName( Form("%s_%f", histo->GetTitle() , gRandom->Uniform()  ) );

        for( Int_t i =binx1; i<=binx2; i++ ){
                float xxx = histo->GetXaxis()->GetBinCenter( i );
                float yyy = histo->GetBinContent( i );

                int   bin_for_histoNew = histoNew->GetXaxis()->FindBin( xxx );
                histoNew->SetBinContent( bin_for_histoNew, yyy);
        }

        histo = (TH1*) histoNew;
        histo_backup =  (TH1*) histoNew->Clone();
        histo->Rebin( fCmp );
        histo->Draw();
        c1->Update();

    }



}

void ROOTSCOPE_lite::To_change_linewidth(){

    fH1_linewidth ++ ;
    if( fH1_linewidth %5 == 0 ) { fH1_linewidth =1; }


    histo->SetLineWidth(fH1_linewidth  );
    histo_backup->SetLineWidth(fH1_linewidth  );

    histo->Draw();
    c1->Update();

}


void ROOTSCOPE_lite::To_change_fillstyle() {

    Color_t fillColor;
    fH1_fillstyle ++ ;
    if( fH1_fillstyle %2 == 0 ) { fH1_fillstyle =0; }

    if( fH1_fillstyle == 1 ) {
        histo->SetFillStyle( 3004 );
        fillColor = histo->GetLineColor();
        histo->SetFillColor( fillColor );
        histo_backup->SetFillStyle( 3004 );
        histo_backup->SetFillColor( fillColor );
        }
    else if ( fH1_fillstyle == 0 ) {
        histo->SetFillStyle( 0 );
        histo_backup->SetFillStyle( 0 );
        }

    histo->Draw();
    c1->Update();


}


void ROOTSCOPE_lite::To_set_logscale(){

    fLogScale_style ++;
    if( fLogScale_style%2 == 0) { fLogScale_style = 0;  }
    // Note: if user set a customized y range, then set log scale may crash
    // so we need to unzoom the y axis first.
    if( fLogScale_style == 1 ) { histo->GetYaxis()->UnZoom(); gPad->SetLogy(kTRUE); }
    if( fLogScale_style == 0 ) { gPad->SetLogy(kFALSE); }


    c1->Update();
}


void ROOTSCOPE_lite::To_writeoutFile() {

     //-------------------------------------------file dialog
    TString     dir(".");
    TGFileInfo  filieInfo;
    const char *filetypes[] = { "ROOT files",    "*.root",
                                0,               0 };

    filieInfo.fFileTypes = filetypes;
    filieInfo.fIniDir    = StrDup(dir);
    new TGFileDialog( gClient->GetRoot(), this , kFDOpen, &filieInfo);
    TString outFileName = filieInfo.fFilename;  // get our output file name~~


    outFileName = outFileName.Strip ( TString::kBoth, ' ' );

    if ( outFileName != "" )
    {

        // a shortcut "x" for using the histogram's title as
        // the output rootfile name.
        Int_t sizeN = outFileName.Length();
        TString fileName = outFileName(sizeN-2, sizeN-1);
        TString filePath = outFileName(      0, sizeN-2);
        if ( fileName == "/x" ) {
            fileName = "/";
            fileName += histo->GetTitle();
            outFileName = filePath + fileName;
            }




        // check the ending is "xx.root"
        sizeN = outFileName.Length();
        TString tmp = outFileName(sizeN-5, sizeN-1);
        if( tmp != ".root") { outFileName += ".root"; }


        // write out the data
        TFile outFile( outFileName.Data(), "RECREATE" );
        if( outFile.IsOpen() )
        {
            histo->Write();
            *fText_viewer << Form("write out to %s", outFileName.Data() )<< endl;
            fText_viewer->ShowBottom();
        }

        outFile.Close();
    }


}

void ROOTSCOPE_lite::To_show_find_peaks_dlg() {


    Dlg_to_find_peaks* testObj
    = new Dlg_to_find_peaks
    ( gClient->GetRoot(), &fSigma_guess, &fThreshold, &fTextSize, &fTextPrecision );



    if( fTextPrecision <= 5 ) {

        fPrecisionFormat = " %.";
        fPrecisionFormat += Form( "%d", static_cast<int>(fTextPrecision) );
        fPrecisionFormat += "f";
    } else {
        fPrecisionFormat = " %.f";
    }



    *fText_viewer
    << Form("Set sigma = %.2f, threshold = %.2f%%, fontSize = %.1f, decimal point = %d",
    fSigma_guess, fThreshold, fTextSize, static_cast<int>(fTextPrecision)  )  <<  endl;
    fText_viewer->ShowBottom();



}


void ROOTSCOPE_lite::To_find_peaks() {


    //
    // clean up previous polymarkers or text labels (if exists)
    //
    TList*  all_items;
    Int_t objN;
    c1->cd( );


    all_items = (TList*) gPad->GetListOfPrimitives();
    objN = all_items->GetSize();

    if( ROOT_VERSION_CODE < ROOT_VERSION(6,0,0) ) {

        //===== for root 5 =====
        for(int i=0; i< objN; i++){
            TObject* element = all_items-> At( i );
            if( element->IsA()== TPolyMarker::Class() ) { element = 0; delete element;  }
        }
        all_items = (TList*) gPad->GetListOfPrimitives();
        objN = all_items->GetSize();
        for(int i=0; i< objN; i++){
            TObject* element = all_items-> At( i );
            if( element->IsA()== TText::Class() ) { element = 0; delete element;   }
        }
    }
    else {
        // =====  for root 6 =====
        for(int i=0; i< objN; i++)
        {
            TObject* element = all_items-> At( i );
            TPolyMarker* obj1 = dynamic_cast<TPolyMarker*> ( element );
            if ( obj1 == nullptr ) continue;
            delete obj1;
        }
        all_items = (TList*) gPad->GetListOfPrimitives();
        objN = all_items->GetSize();
        for(int i=0; i< objN; i++) {

            TObject* element = all_items-> At( i );
            TText* obj2 = dynamic_cast<TText*> ( element );
            if ( obj2 == nullptr ) continue;
            delete obj2;
        }
    }



    // to use TSpectrum to get peaks.( Max peaks = 100 )
    const Int_t     nfoundMax = 100;

    Int_t nfound = peakFinder -> Search( histo, fSigma_guess, "goff", fThreshold );
    Float_t    peak_pos[ nfoundMax ] = {0};
    Float_t    peak_y[ nfoundMax ] = {0};


    TPolyMarker* tmp_pm = new TPolyMarker();
    tmp_pm->SetMarkerStyle(23);
    tmp_pm->SetMarkerSize(2);
    tmp_pm->SetMarkerColor( kPink - 4 );
    tmp_pm->SetBit( kCannotPick  );

    TList* tmp_texts = new TList();


    for( Int_t i=0; i<nfound; i++) {
        peak_pos[i] = peakFinder-> GetPositionX()[i];
        peak_y[i]   = peakFinder-> GetPositionY()[i];

        // To assign the text labels.
        TText* tmp_text = new TText();
        tmp_text->SetTextFont(43);
        tmp_text->SetTextSize( fTextSize );
        tmp_text->SetTextColor( kRed  );
        tmp_text->SetBit( kCannotPick  );
        //tmp_text->SetText (peak_pos[i], peak_y[i],  Form(" %.f", peak_pos[i]) );
        tmp_text->SetText (peak_pos[i], peak_y[i],
            Form(fPrecisionFormat.Data(), peak_pos[i]) );
        tmp_texts->Add( (TText*) tmp_text->Clone() );
        delete tmp_text;

        // to assign polymakers
        tmp_pm->SetPoint(i, peak_pos[i], peak_y[i] );
    }


    // reassign to fText and fPolyMarkers, then delete the tmp ones.
    fTexts = (TList*) tmp_texts->Clone();
    delete tmp_texts;

    fPolyMarkers = (TPolyMarker*) tmp_pm->Clone( );
    delete tmp_pm;

    // ploting texts/ polymarkers
    Int_t textN = fTexts->GetSize();
    for(int i=0; i< textN; i++){ fTexts->At(i)->Draw( "same" ); }
    fPolyMarkers->Draw( );


    c1->Update();

}



void ROOTSCOPE_lite::SetOneDHisto( TH1* h_input ) {

    histo = h_input;

    histo_backup = (TH1*) h_input->Clone(); // backup, will be used when rebin.

    c1->Update();
}


TH1* ROOTSCOPE_lite::GetOneDHisto(){

    return histo;

}


// constructor
ROOTSCOPE_lite::ROOTSCOPE_lite( const TGWindow * p, TH1* histo_input )
: TGMainFrame(p, 500, 400)
{
    SetCleanup(kDeepCleanup); // important step for closing windows properly.

    fH1_fillstyle = 0;
    fH1_linewidth = 1;
    fLogScale_style = 0;
    fCmp = 1;

    const Int_t nfoundMax = 100;
    peakFinder = new TSpectrum( nfoundMax );
    fSigma_guess = 0.2;
    fThreshold   = 0.1;
    fTextSize    = 16;
    fTextPrecision = 0;
    fPrecisionFormat = " %.f";



    myline1 = new TLine();
    myline2 = new TLine();
    general_line1 = new TLine();
    general_line1->SetLineColor( kGreen );
    general_line1->SetLineStyle( 2 );
    general_line2 = new TLine();
    general_line2->SetLineColor( kGreen );
    general_line2->SetLineStyle( 2 );

    fWhich_active_line = 1;
    fXrange_pick1 = 0;
    fXrange_pick2 = 0;
    fGeneral_pick1 = 0;
    fGeneral_pick2 = 0;
    fBG_const  = 0.0;
    fBG_linear = 0.0;
    simple_gaus = new TF1("simple_gaus", "gaus", fXrange_pick1, fXrange_pick2 );

    fTF1_bg_const  = new TF1("background_pol0", "pol0");
    fTF1_bg_linear = new TF1("background_pol1", "pol1");
    fTF1_gaus_bg   = new TF1("gaus+bg", gauss_bg, fXrange_pick1, fXrange_pick2, 5 );
    fTF1_gaus_bg->SetNpx( 500 );
    fTF1_gaus_bg_2 = new TF1("gaus+bg2", gauss_bg, fXrange_pick1, fXrange_pick2, 5 );
    fTF1_gaus_bg_2 ->SetNpx( 500 );

    fTF1_double_gaus_bg
    = new TF1( "double_gauss_bg", double_gauss_bg, fXrange_pick1, fXrange_pick2, 8 );
    fTF1_double_gaus_bg->SetNpx( 500 );


    histo = histo_input;
    histo_backup = (TH1*) histo_input->Clone(); // backup, will be used when rebin.

    //------------------------------------------------------------------|



    fText_viewer =  new TGTextViewostream (	this, /* parent */
                                            400, 100 ); /* width, height */


    AddFrame( fText_viewer,  new TGLayoutHints( kLHintsExpandX , 2,2,0,0) );






    //------------------------------------------------------------------|

    emb_Canvas =
    new TRootEmbeddedCanvas(    "emb_Canvas",  /* name of embedded canvas*/
                                this,   /* parent frame */
                                500,      /* width */
                                400);     /* height */

    /*
    |   create a signal/slot connection
    |   Connect is a public method,  from TGMainFrame inherited from TQObject cleas.
    |   For embedded calss, events are processed by the container.
    |   Don't use emb_Canvas->Connect( .. )
    */
    emb_Canvas->GetContainer()->Connect(   "ProcessedEvent(Event_t*)",  /*singal*/
                                            "ROOTSCOPE_lite",           /*receiver class*/
                                            this,                       /*receiver obj*/
                                            "To_response(Event_t*)");   /*slot*/


    c1 = emb_Canvas->GetCanvas();
    To_Update_Canvas();
    AddFrame(emb_Canvas,
            new TGLayoutHints( kLHintsExpandX | kLHintsExpandY) );

    //------------------------------------------------------------------|




    // routine stuffs...
    SetWindowName("ROOTSCOPE_lite");    // Set a name to the main frame
    MapSubwindows();                    // Map all subwindows of main frame
    Resize( GetDefaultSize() );         // Initialize the layout algorithm
    MapWindow();                        // Map main frame


}



void myROOTSCOPE_lite() {

    TH1F* histo;
    histo = new TH1F("h","h",100,-3,3);
    histo->FillRandom("gaus",1000);


    ROOTSCOPE_lite* app = new ROOTSCOPE_lite ( gClient->GetRoot(), histo );


}
