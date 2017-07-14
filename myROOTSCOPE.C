//
//   Author : Pei-Luan Tai
//   Contact: pt10f@my.fsu.edu
//   Last update: May 2, 2017
//***************************************/
#include "myDialog.h"
#include "myUtility.h"

#include <RVersion.h>
#include <TGaxis.h>
#include <TStyle.h>
#include <RVersion.h>
#include <TGWindow.h>
#include <TGFrame.h>
#include <TLine.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TRandom.h>
#include <TText.h>
#include <TSpectrum.h>
#include <TPolyMarker.h>
#include <vector>
#include <TVirtualPad.h>
#include <TList.h>
#include <TString.h>
#include <TObject.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TFile.h>
#include <TBox.h>
#include <TRootEmbeddedCanvas.h>
#include <TGTextEntry.h>
#include <TGNumberEntry.h>
#include <TGTextViewStream.h>


using namespace std;







class ROOTSCOPE : public TGMainFrame {

private:
    //---------------for 1d histo----------------------------//

    int     fCmp;                       /* rebin value */
    int     fWhich_active_line;         /* to set which line is active */
    float   fXrange_pick1;              /* range1 set by mouse click*/
    float   fXrange_pick2;              /* range2 set by mouse click*/
    float   fGeneral_pick1;             /* multi-purpose */
    float   fGeneral_pick2;             /* multi-purpose */

    float   fBG_const;                  /* bg value, const term */
    float   fBG_linear;                 /* bg value, linear term */

    int     fPadTotalN;                  /* the total pad numbers */
    int     fPadActive;                  /* the current pad number */
    bool    fIsSameDim;                  /* is display 1d histos have same dim? */
    bool    fIsTH2_inPad;                /* a flag to show whether TH2 object in the pad */
    bool    fHasTH2;                     /* a flag check we have TH2 from input */

    TH1*    histo;                       /* the 1D histo */
    TH1*    histo_backup;                /* the 1D histo backup*/

    Int_t   fSwitch_overlap;            /* switch for overlap mode.*/
    Int_t   fOverlaph1Idx;
    Int_t   fOverlaph2Idx;
    Bool_t  fOverlapMode;
    TGaxis* faxis;

    TLine*  myline1;                     /* line marker 1 */
    TLine*  myline2;                     /* line marker 2 */
    TLine*  general_line1;
    TLine*  general_line2;

    Int_t   fH1_fillstyle;               /* fill style */
    Int_t   fH1_linewidth;               /* line width */

    TList*          fTexts;             /* text labels for found peaks*/
    Float_t         fTextSize;
    Float_t         fTextPrecision;
    TString         fPrecisionFormat;
    TPolyMarker*    fPolyMarkers;       /* markers for found peaks*/
    TSpectrum*      peakFinder;
    Float_t         fSigma_guess;       /* parameters for finding peaks*/
    Float_t         fThreshold;         /* parameters for finding peaks*/



    //---------------for 2d histo----------------------------//
    TH2*           histo2d;            /* the 2D histo  */
    TH2*           histo2d_backup;     /* the 2D histo backup */

    Int_t  fCmp_2d_X;                   /* rebin value fro 2d  x axis */
    Int_t  fCmp_2d_Y;                   /* rebin value fro 2d  x axis */
    Int_t  fWhich_active_2d;            /* which marker set is active 1 or 2  */
    TLine* fH2_marker_setA[2];          /* line marker set A  */
    TLine* fH2_marker_setB[2];          /* line marker set A  */
    float  fH2_pickX[2];                /* x pick by mouse click*/
    float  fH2_pickY[2];                /* y pick by mouse click*/

    Int_t               fLogScale_style;    /* switch for log scale. */
    Int_t               fH2_style;          /* the draw style for TH2 */
    Int_t               fH2_marker_style;   /* the marker style for TH2 */
    static const char*  styleDraw[3];
    static const Int_t  styleMarkersize[5];



    //---------------for the array of histos---------------------//

    vector<TH1*>        histos;              /* the 1D histo created from histo2d */
    vector<TH1*>        histos_backup;       /* backups that used in compress/uncompress */

    vector<int>         fOpen_list;          /* the list of idx for the diplayed histograms */

    Int_t               ftemp_lineN;
    TLine*              fTmpSet1_lines[10];   /* for temp lines */
    TLine*              fTmpSet2_lines[10];   /* for temp lines */



    //---------------for fitting 1d histo---------------------//
    TF1*    simple_gaus;                /* only for testing*/
    TF1*    fTF1_bg_const;              /* bg = const */
    TF1*    fTF1_bg_linear;             /* bg = const + linear*x */
    TF1*    fTF1_gaus_bg;               /* y = guas(x) + a*x + c */
    TF1*    fTF1_gaus_bg_2;
    TF1*    fTF1_double_gaus_bg;


    //---------------others-----------------------------------//

    TRootEmbeddedCanvas *emb_Canvas;     /* Canvas container */
    TCanvas             *c1;             /* Canvas  */
    TGTextViewostream   *fText_viewer;   /* for show user message */

    void To_backup_histos();

    void To_writeoutFile();

    void To_readinFile();


    //----------------------------------------- 1d histo

    void SetMarker(Event_t*);

    void SetMarker2(Event_t*);

    void SetGeneralMarker(Event_t*);

    void Clear_Marker();

    void Set_Y_Range();

    void Expand();

    void Expand_dlg();

    void Unexpand();

    void Go_half_left();

    void Go_half_right();

    void Rebin_compress();

    void Rebin_uncompress();

    void Set_histo_title();

    void To_find_peaks();

    void To_show_find_peaks_dlg();



    void To_show_histo_dlg();

    void To_show_histo_operation_dlg( );

    void To_display_histos(  bool showMSG   );

    void To_switch_histo( bool toIncrease );

    void To_delete_histo_inSelectedPad();

    void To_Change_histo_x_range();

    void To_overlap_histos();

    void _doing_overloap( TH1* h1, TH1* h2, Int_t h1Idx, Int_t h2Idx, Bool_t ShowMsg );

    void To_switch_overlap();

    void To_change_linewidth();

    void To_change_fillstyle();


    void Fit_Gaussian();

    void Fit_Double_Gaussian();

    void Fit_Background();       // for bg = const

    void Fit_Background_pol1();  // for bg = const + linear * x

    void Set_Background();

    void Get_Sum( bool isTH2  ); // for both 1d and 2d

    void To_show_AddSub_gate_dlg();

    void Project_from_1d( bool flagXY );


    //-------------------------------------- 2d histo

    void Projection_2d( bool flagXY );

    void SetMarker_2d( Event_t* );

    void Clear_Marker2d( );

    void Expand_2d();

    void Unexpand_2d();

    void Set_Z_Range( bool toSetMax );

    void Rebin_2d(bool flagXY, bool flag2 );

    void To_display_histo2d();

    void To_Back_to_disply_histos();


    void To_change_style_2d();

    void To_change_marker_size();

    void To_set_logscale(); //also for 1d




    //----------------------------------------- utility

    void  Get_active_histo1D();

    float Get_Max_range ( float, float);

    float Get_Min_range ( float, float);

    void  Set_Xrange_pick( float );

    void  SetActiveLine( TLine* , float, float );

    void  Switch_ActiveLine(TLine*, TLine*);

    void  Set_Line( TLine*, float x1, float y1, float x2, float y2 );

    void  To_Draw_bg();

    void  Set_init_fit_value( double, double, double& , double& , double& );

    TH1* Get_histo_by_padnum( int padnum);

    void Single_pad_setting();

    void Set_CannotMove();

    void Initialization();

    void Create_Widgets( UInt_t w, UInt_t h  );

    bool Remove_default_empty_histo();

    Dlg_Set_Background* fdlg_test;


public:



    // constuctor: widgets are placed here.

    ROOTSCOPE( const TGWindow * p,  TH1*  );

    ROOTSCOPE( const TGWindow * p,  TH2*  );

    ROOTSCOPE( const TGWindow * p, const char* rootFileName  );

    ROOTSCOPE( const TGWindow * p  );



    void To_response(Event_t*);  // to response key press from user

    TH2* GetTwoDHisto();

    TH1* GetOneDHisto();

    void SetTwoDHisto( TH2* h2d_input );

    void AddOneDHisto( TH1* h_input );

    TString GetMessage(){ return fText_viewer->GetText()->AsString();  }

    TCanvas* GetCanvas() { return c1; }

};



const char* ROOTSCOPE::styleDraw[3] = {"",  "COL2", "COLZ2" };

const int   ROOTSCOPE::styleMarkersize[5] = { 1, 6 , 7,  20 };





void ROOTSCOPE::To_response(Event_t* e) {

    // the center for key and mouse response.

    /*
        Note: Event_t data type see:
        https://root.cern.ch/root/html/Event_t.html
        for mouse click : check EEventType

        Note: for key symbols
        https://root.cern.ch/doc/v608/KeySymbols_8h.html
        https://root.cern.ch/doc/v606/GuiTypes_8h.html
    */

    Set_CannotMove();

    // to examine whether a TH1 or TH2 obj in the click-selected pad
    // and then set the corresponding flags.
    if ( e->fType == kButtonPress && e->fCode == kButton1Down )
    { Get_active_histo1D(); }



    // key code lookup table.
    const unsigned key_arrow_up    = 111;
    const unsigned key_arrow_down  = 116;
    const unsigned key_arrow_left  = 113;
    const unsigned key_arrow_right = 114;


    // for key symbol look up.
    UInt_t key_symbol;
    char tmp[2];
    if ( e->fType == kGKeyPress )
    {  gVirtualX->LookupString( e, tmp,sizeof(tmp), key_symbol ); }


    // Alt and Ctrl
    bool isCtrl = false;
    bool isAlt  = false;
    if ( e->fState & kKeyMod1Mask )    { isAlt=true; }    else { isAlt = false;  }
    if ( e->fState & kKeyControlMask ) { isCtrl=true; }   else { isCtrl = false; }



    // the object in the pad is TH1.
    if( !fIsTH2_inPad ) {

        // for mouse button
        if ( e->fType == kButtonPress && e->fCode == kButton1Down && !isCtrl && !isAlt   )
        { SetMarker( e ); } // right click to set a marker.

        // ctl + right click.
        if ( e->fType == kButtonPress && e->fCode == kButton1Down && isCtrl && !isAlt   )
        { SetGeneralMarker( e ); }

        //  for key press test
        //  cout << "test e->fCode = "<< e->fCode << endl;

        //_______key
        if ( e->fType == kGKeyPress && !isCtrl && !isAlt  )  {

            if( key_symbol == kKey_m  ) { SetMarker( e ); }

            else if( key_symbol == kKey_n )  { Clear_Marker(); }

            else if( key_symbol == kKey_e )  { Expand(); }

            else if( key_symbol == kKey_E )  { Expand_dlg(); }

            else if( key_symbol == kKey_o )  { Unexpand( ); }

            else if( key_symbol == kKey_y )  { Set_Y_Range( ); }

            else if( e->fCode == key_arrow_left )  { Go_half_left( ); }

            else if( e->fCode == key_arrow_right )  { Go_half_right( ); }

            else if( e->fCode == key_arrow_up )    { To_switch_histo(1); }

            else if( e->fCode == key_arrow_down )  { To_switch_histo(0); }



            else if( key_symbol == kKey_p  ) { Project_from_1d( 1 ); }

            else if( key_symbol == kKey_P  ) { Project_from_1d( 0 ); }

            else if( key_symbol == kKey_f  ) { To_find_peaks(); }




            else if ( key_symbol == kKey_b  ) { Fit_Background(); }  // fit bg by bg = const.

            else if ( key_symbol == kKey_1 )  { Fit_Background_pol1(); } // fit bg by bg = const + linear* x.

            else if( key_symbol == kKey_g )  { Fit_Gaussian( ); }

            else if( key_symbol == kKey_G )  { Fit_Double_Gaussian( ); }

            else if( key_symbol == kKey_s )  { Get_Sum( 0 ); }

            else if( key_symbol == kKey_c )  { To_switch_overlap(); }

            else if( key_symbol == kKey_x ) { Rebin_compress();    }

            else if( key_symbol == kKey_X ) { Rebin_uncompress(); }

            else if( key_symbol == kKey_d  ) { To_show_histo_dlg( ); }

            else if( key_symbol == kKey_V  ) { To_show_histo_operation_dlg( ); }

            else if( key_symbol == kKey_F1  ) { To_change_fillstyle(); }

            else if( key_symbol == kKey_F2  ) { To_change_linewidth(); }

            else if( key_symbol == kKey_F3  ) { To_set_logscale(); }


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

            // ctl + c
            if( key_symbol == kKey_c ) { To_delete_histo_inSelectedPad(); }

            // ctl + d
            if( key_symbol == kKey_d ) { To_overlap_histos(); }

            // ctl + w
            else if( key_symbol == kKey_w  ) { To_writeoutFile(); }

            // ctl + r
            else if( key_symbol == kKey_r  ) { To_readinFile(); }

            // ctl + q
            else if( key_symbol == kKey_q  ) { To_display_histo2d(); }

            // ctl + f
            else if( key_symbol == kKey_f  ) { To_show_find_peaks_dlg(); }

            // ctl + p
            else if( key_symbol == kKey_p  ) { To_show_AddSub_gate_dlg(); }

        }

    } //-------------------------------------end of key reponses for TH1 obj


    // the object in the pad is TH2.
    if( fIsTH2_inPad ) {


        // for mouse button
        if (  e->fCode == kButton1Down && e->fType == kButtonPress && isCtrl && !isAlt  )
        { SetMarker_2d( e ); } // ctl + right click to set a marker.

        //_______key
        if ( e->fType == kGKeyPress && !isCtrl && !isAlt ) {

            if( key_symbol == kKey_d  ) { To_show_histo_dlg( ); }

            else if( key_symbol == kKey_V  ) { To_show_histo_operation_dlg( ); }

            else if( key_symbol == kKey_q  ) { To_Back_to_disply_histos(); }

            else if( key_symbol == kKey_p  ) { Projection_2d( 1 ); }

            else if( key_symbol == kKey_P  ) { Projection_2d( 0 ); }

            else if( key_symbol == kKey_n  ) { Clear_Marker2d( ); }

            else if( key_symbol == kKey_e  ) { Expand_2d( ); }

            else if( key_symbol == kKey_o  ) { Unexpand_2d( ); }

            else if( key_symbol == kKey_F1  ) { To_change_style_2d( ); }

            else if( key_symbol == kKey_F2  ) { To_change_marker_size( ); }

            else if( key_symbol == kKey_F3  ) { To_set_logscale( ); }


            else if( key_symbol == kKey_s )  { Get_Sum(1); }

            else if( key_symbol == kKey_z )  { Set_Z_Range(1); } // set z max

            else if( key_symbol == kKey_Z )  { Set_Z_Range(0); } // set z min


            else if( key_symbol == kKey_x ) { Rebin_2d(1, 1); }

            else if( key_symbol == kKey_X ) { Rebin_2d(1, 0); }

            else if( key_symbol == kKey_y ) { Rebin_2d(0, 1); }

            else if( key_symbol == kKey_Y ) { Rebin_2d(0, 0); }




        }

        //_______ctrl + key
        if ( e->fType == kGKeyPress && isCtrl && !isAlt   ) {

            // ctl + w
            if( key_symbol == kKey_w  ) { To_writeoutFile(); }

            // ctl + r
            else if( key_symbol == kKey_r  ) { To_readinFile(); }
        }


    } //--------------------------------- end of key reponses for TH2 obj


}



void ROOTSCOPE::SetGeneralMarker( Event_t* e) {


    TVirtualPad* currentPad = c1->GetClickSelectedPad();

    int   px = e->fX;
    float xx = currentPad->AbsPixeltoX(px);
    int binx = histo->GetXaxis()-> FindBin(xx);

    float x_pos = histo-> GetXaxis()-> GetBinCenter( binx );
    float y_pos1 =  currentPad->GetUymin();
    float y_pos2 =  currentPad->GetUymax();

    if  ( x_pos != fGeneral_pick1 )
    {
        fGeneral_pick2 = fGeneral_pick1;
        fGeneral_pick1 = x_pos;
        currentPad->cd();

        Set_Line( general_line1, x_pos, y_pos1, x_pos, y_pos2 );
        Set_Line( general_line2, fGeneral_pick2, y_pos1, fGeneral_pick2, y_pos2 );
        general_line1->Draw();
        general_line2->Draw();
    }

    c1->Update();
}


void ROOTSCOPE::SetMarker(Event_t* e){

    //------------------------------------------------------------------| get x_pos, y_pos.
    TVirtualPad* currentPad = c1->GetClickSelectedPad();

    Int_t   px = e->fX;
    Float_t xx = currentPad->AbsPixeltoX(px);
    Int_t binx = histo->GetXaxis()-> FindBin(xx);

    Float_t x_pos = histo-> GetXaxis()-> GetBinCenter( binx );
    Float_t y_pos =  currentPad->GetUymax();




    //-------------------------------------------------| set x_pos, y_pos to active line.
    Switch_ActiveLine( myline1, myline2 ); //switch between fWhich_actice_line.

    if ( fWhich_active_line == 1 )        { SetActiveLine( myline1, x_pos, y_pos ); }

    else if ( fWhich_active_line  == 2  ) { SetActiveLine( myline2, x_pos, y_pos ); }

    /*  The idea here is :

        Initially we have both myline1 and myline2 set black color, and fWhich_active_line = 2
        so the first click will go to esle if statement, and plot the black myline2.

        The second click, in the Switch_ActiveLine function, set myline2 orange, and myline1 black,
        and set fWhich_active_line = 1. and hence we plot black myline1, and the previous myline2 becomes orange.

        The thrird click, set myline2 black, myline1 oragne, fWhich_active_line = 2
        then we plot myline2, and the previous myline1 becomes orange.

        if we click on the same bin twice, it only set fWhich_active_line = 2,
        and only plot black myline1.
    */

    Set_Xrange_pick( x_pos ); // update fXrange_pick1 and fXrange_pick2;



    // get spectrum num
    Int_t specNum;
    if( fPadTotalN == 1 ){ specNum = fOpen_list.at(0) + 1;  }
    else if ( fPadTotalN > 1 ) { specNum = fOpen_list.at( fPadActive-1) + 1; }


    // show message
    float y_counts = histo-> GetBinContent( binx );
    *fText_viewer
    << Form("x = %6.2f (bin=%5d), y = %6.2f, spec#%02d ",
    x_pos, binx, y_counts, specNum )  <<  endl;
    fText_viewer->ShowBottom();



}



void ROOTSCOPE::SetMarker2(Event_t* e){



    // we will pop up a dialog for user to set the marker

    //-------------------------------------------------------| get x_pos, y_pos.
    TVirtualPad* currentPad = c1->GetPad( fPadActive );
    float x_pos;
    float x_pos_limit1 =  histo->GetXaxis()->GetXmin();
    float x_pos_limit2 =  histo->GetXaxis()->GetXmax();

    // open a dialog for user to input marker position ( x_pos )
    new Dlg_Set_Marker( gClient->GetRoot(), this,
                        10, 10,
                        &x_pos, x_pos_limit1, x_pos_limit2 );

    float y_pos =  currentPad->GetUymax();




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
        *fText_viewer << Form("x = %6.2f\t\t(bin \t= %d) ", x_pos, binx)  <<  endl;
        fText_viewer->ShowBottom();

    }

}



void ROOTSCOPE::Set_Y_Range( ) {

    /*  only apply for the click-selected pad.
        we disable Set_Y_Range() function in the logscale mode.
    */

    if ( fLogScale_style != true ) {

        TVirtualPad* currentPad = c1->GetPad( fPadActive );

        float y_max;

        // passing by reference to get y_max from user input
        new Dlg_Set_YRange(gClient->GetRoot(), this, 10, 10,
                        &y_max , currentPad->GetUymax() );

        histo->GetYaxis()->SetRangeUser( 0, y_max );
        c1->cd( fPadActive);
        histo->Draw();
        c1->Update();

    } else{

        *fText_viewer <<
        Form("In log scale mode, set y range will have an issue in display.")  <<  endl;
        fText_viewer->ShowBottom();

    }

}


void ROOTSCOPE::Clear_Marker(){

    // hide the line makers, and clear the fitting lines.

    Set_Line( myline1, 0, 0, 0 ,0 ); myline1->Draw();
    Set_Line( myline2, 0, 0, 0 ,0 ); myline2->Draw();

    if ( fPadTotalN > 1 ) {

        TH1* htest1;
        for( int i=1; i<= fPadTotalN; i++ ) {
            c1->cd(i);
            Set_Line( fTmpSet1_lines[i], 0, 0, 0 ,0 );
            Set_Line( fTmpSet2_lines[i], 0, 0, 0 ,0 );
            fTmpSet1_lines[i]->Draw();
            fTmpSet2_lines[i]->Draw();
            Get_histo_by_padnum( i )->Draw();
            c1->GetPad(i)->Update();
        }
    }


    c1->cd( fPadActive);
    histo->Draw();
    c1->Update();

}





void ROOTSCOPE::Set_Line( TLine* l, float x1, float y1, float x2, float y2 ) {

    l->SetX1( x1 ); l->SetY1( y1 );
    l->SetX2( x2 ); l->SetY2( y2 );

}


void ROOTSCOPE::Switch_ActiveLine(TLine* l1, TLine* l2) {

    float l1_x = l1->GetX1();
    float l2_x = l2->GetX1();


    if ( l1_x != l2_x ) {

        if ( fWhich_active_line == 1 ) {

            l1->SetLineColor( kOrange );
            l2->SetLineColor( kBlack );
            for( int i=0; i< ftemp_lineN; i++ ) { fTmpSet1_lines[i]->SetLineColor( kOrange ); }
            for( int i=0; i< ftemp_lineN; i++ ) { fTmpSet2_lines[i]->SetLineColor( kBlack ); }
            fWhich_active_line = 2;


        }
        else if ( fWhich_active_line == 2) {
            l1->SetLineColor( kBlack );
            l2->SetLineColor( kOrange );

            for( int i=0; i< ftemp_lineN; i++ ) { fTmpSet1_lines[i]->SetLineColor( kBlack ); }
            for( int i=0; i< ftemp_lineN; i++ ) { fTmpSet2_lines[i]->SetLineColor( kOrange); }
            fWhich_active_line = 1;

        }

    } else {
        // for the case when we click on the same bin twice ( l1_x == l2_x )
        // or initially l1_x =l2_x = 0

        fWhich_active_line = 2;
    }

}

void ROOTSCOPE::SetActiveLine( TLine* l, float x_pos, float y_pos ) {

    /*  (a) when we have two or more pads display in the canvas, and
        (b) when all the displayed 1d histos have same x dimension.
        we don't use myline1 and myline2 to plot the markers,
        instead we use two sets of temp lines (fTmpSet1_lines[] and fTmpSet2_lines),
        such that each pad have two lines.

        the variable "l" still has to be set, since it
        record where the current marker, but just not involves in the plotting.
    */



    if( fPadTotalN == 1 )
    {

        c1->cd();
        Set_Line( l, x_pos, 0, x_pos, y_pos );  l->Draw();
        c1->Update();
    }


    // (a) when we have two or more pads display in the canvas
    if ( fPadTotalN > 1 )
    {

        // here l is only for record  but not plotting
        Set_Line( l, x_pos, 0, x_pos, y_pos );



        if( fIsSameDim ) {
            // (b) when all the displayed 1d histos have same x dimension.
            //---------------------------------------------------------------------------------|
            if( fWhich_active_line == 1 )  {

                for( int i = 1; i <= fPadTotalN; i++ ) {
                    // note: index starts from 1
                    c1->cd(i);
                    Set_Line( fTmpSet1_lines[i], x_pos, 0, x_pos, c1->GetPad(i)->GetUymax() );
                    fTmpSet1_lines[i]->Draw();
                }
            }
            else if ( fWhich_active_line == 2 ){

                for( int i = 1; i<=fPadTotalN; i++ ) {
                    // note: index starts from 1
                    c1->cd(i);
                    Set_Line( fTmpSet2_lines[i], x_pos, 0, x_pos, c1->GetPad(i)->GetUymax() );
                    fTmpSet2_lines[i]->Draw();
                }
            }
            //---------------------------------------------------------------------------------|
            c1->Update();
            c1->cd( fPadActive ); // return the our selected pad.

        }
        else {

            // when we don't have all the histos with same x dimensions.
            // then we just plot the selected pad by myline1 and myline2
            c1->cd( fPadActive );
            Set_Line( l, x_pos, 0, x_pos, y_pos );  l->Draw();
            l->Draw();
            c1->Update();
        }

    }

}







TH1* ROOTSCOPE::Get_histo_by_padnum( int padnum) {

    TVirtualPad*    myPad  = c1->GetPad( padnum );
    TList*          myList = myPad->GetListOfPrimitives();
    Int_t           objN   = myList->GetSize();

    TH1* htemp;     // just for looping
    TH1* hresult;

    for(int i=0; i< objN; i++)
    {
        TObject* element = myList-> At( i );
        htemp = dynamic_cast<TH1*> ( element );
        if ( htemp == nullptr ) continue;
        hresult = htemp;
    }

    return hresult;

}


void ROOTSCOPE::Get_active_histo1D() {

    /*  Here we determine fPadActive,
        to filter out the TH1 object, and assign it to histo,
        in case the object is TH2, we set fIsTH2_inPad = true.
    */

    TVirtualPad* selectedPad = c1->GetClickSelectedPad();

    // Grad all the objects in Click-selected Pad
    TList* myList = selectedPad->GetListOfPrimitives();
    int objN = myList->GetSize();


    fPadActive = selectedPad->GetNumber();  // the current pad number



    TH1* htemp;
    TH2* h2temp; fIsTH2_inPad = false;

    //  to filter out the TH1 object, and assign to histo
    for(int i=0; i< objN; i++)
    {
        TObject* element = myList-> At( i );

        htemp = dynamic_cast<TH1*> ( element );
        if ( htemp == nullptr ) continue;
        histo = htemp; // grab the TH1 address to "histo"
    }

    //  to filter out the TH2 object
    for(int i=0; i< objN; i++)
    {
        TObject* element = myList-> At( i );

        h2temp = dynamic_cast<TH2*> ( element );
        if ( h2temp == nullptr ) continue;
        fIsTH2_inPad = true;
    }



    c1->cd( fPadActive );


}



void ROOTSCOPE::Initialization() {


    const Int_t nfoundMax = 100;
    peakFinder = new TSpectrum( nfoundMax );

    fSigma_guess = 0.2;
    fThreshold   = 0.1;
    fTextSize    = 16;
    fTextPrecision = 1;
    fPrecisionFormat = " %.1f";


    fSwitch_overlap = 1;
    fOverlapMode = false;
    fCmp = 1;
    fH2_style = 0;
    fH2_marker_style = 0;
    fLogScale_style = 0;
    fH1_fillstyle = 0;
    fH1_linewidth = 1;
    myline1 = new TLine();
    myline2 = new TLine();
    general_line1 = new TLine();
    general_line1->SetLineColor( kGreen );
    general_line1->SetLineStyle( 2 );


    general_line2 = new TLine();
    general_line2->SetLineColor( kGreen );
    general_line2->SetLineStyle( 2 );

    ftemp_lineN = 10;
    for( int i=0; i< ftemp_lineN; i++ ) { fTmpSet1_lines[i] = new TLine(); }
    for( int i=0; i< ftemp_lineN; i++ ) { fTmpSet2_lines[i] = new TLine(); }
    fWhich_active_line = 1;
    fXrange_pick1 = 0;
    fXrange_pick2 = 0;
    fGeneral_pick1 = 0;
    fGeneral_pick2 = 0;

    for( int i=0; i<2; i++ ) {
        fH2_marker_setA[i] = new TLine(); fH2_marker_setB[i] = new TLine();
        fH2_marker_setA[i]-> SetLineStyle(2);fH2_marker_setB[i]->SetLineStyle(2);

        fH2_pickX[i] = 0;
        fH2_pickY[i] = 0;
    }
    fWhich_active_2d = 1;
    fCmp_2d_X = 1;
    fCmp_2d_Y = 1;



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

    gStyle->SetOptStat(kFALSE);
}



void ROOTSCOPE::Create_Widgets( UInt_t w, UInt_t h  ) {

    /* create widgets and link c1 canvas */

     //------------------------------------------------------------------|
    fText_viewer =  new TGTextViewostream (	this, 400, 100 );
                                            /* parent, width, height */


    AddFrame( fText_viewer,  new TGLayoutHints( kLHintsExpandX , 2,2,0,0) );


    //------------------------------------------------------------------|

    emb_Canvas =
    new TRootEmbeddedCanvas(    "emb_Canvas",  /* name of embedded canvas*/
                                this,   /* parent frame */
                                w,      /* width */
                                h);     /* height */

    /*
    |   create a signal/slot connection
    |   Connect is a public method,  from TGMainFrame inherited from TQObject cleas.
    |   For embedded calss, events are processed by the container.
    |   Don't use emb_Canvas->Connect( .. )
    */
    emb_Canvas->GetContainer()->Connect(   "ProcessedEvent(Event_t*)", /*singal*/
                                            "ROOTSCOPE",    /*receiver class*/
                                            this,           /*receiver obj*/
                                            "To_response(Event_t*)");  /*slot*/

    c1 = emb_Canvas->GetCanvas();

     AddFrame(emb_Canvas,
            new TGLayoutHints( kLHintsExpandX | kLHintsExpandY) );

    //------------------------------------------------------------------|



    // routine stuffs...
    SetWindowName("ROOTSCOPE");    // Set a name to the main frame
    MapSubwindows();               // Map all subwindows of main frame
    Resize( GetDefaultSize() );    // Initialize the layout algorithm
    MapWindow();                   // Map main frame
}




void ROOTSCOPE::Set_Z_Range( bool toSetMax ) {


    // pop up a dlg for user to input Z max value.
    if( toSetMax == true ) {

        float zrange_max;
        float zrange__max_orignal = histo2d->GetMaximum();
        new Dlg_Set_ZRange( gClient->GetRoot(), 10, 10,
                            &zrange_max, zrange__max_orignal, toSetMax ) ;
        histo2d->SetMaximum( zrange_max );

    }

    // pop up a dlg for user to input Z min value.
    if( toSetMax == false )
    {
        float zrange_min;
        float zrange__min_orignal = histo2d->GetMinimum();
        new Dlg_Set_ZRange( gClient->GetRoot(), 10, 10,
                            &zrange_min, zrange__min_orignal, toSetMax ) ;
        histo2d->SetMinimum( zrange_min );

    }

    histo2d->Draw( styleDraw[fH2_style]  );
    c1->Update();
}




void ROOTSCOPE::Get_Sum( bool isTH2 ) {

    // get area sum over a selected range.


    // for TH1 case: report the area above the bg.
    if( !isTH2 )
    {

         // organize the range
        float xMax = Get_Max_range( fXrange_pick1, fXrange_pick2 );
        float xMin = Get_Min_range( fXrange_pick1, fXrange_pick2 );

        int binx1 = histo->GetXaxis()-> FindBin(xMin);
        int binx2 = histo->GetXaxis()-> FindBin(xMax);

        float   area = 0;
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


    // for TH2 case: report the counts in a selected region.
    if( isTH2 )
    {
         // organize the range
        float xMax = Get_Max_range( fH2_pickX[0], fH2_pickX[1] );
        float xMin = Get_Min_range( fH2_pickX[0], fH2_pickX[1] );
        int binx1 = histo->GetXaxis()-> FindBin(xMin);
        int binx2 = histo->GetXaxis()-> FindBin(xMax);

        float yMax = Get_Max_range( fH2_pickY[0], fH2_pickY[1] );
        float yMin = Get_Min_range( fH2_pickY[0], fH2_pickY[1] );
        int biny1 = histo->GetYaxis()-> FindBin(yMin);
        int biny2 = histo->GetYaxis()-> FindBin(yMax);

        if( (xMax != xMin) || (yMax != yMin) )
        {

            Int_t counts = 0;

            for( Int_t i =binx1; i<=binx2; i++ ){
            for( Int_t j =biny1; j<=biny2; j++ ){

                 counts +=  histo2d-> GetBinContent( i, j );
            }}


            *fText_viewer << Form("Counts = %d in the selected region.", counts )  <<  endl;
            fText_viewer->ShowBottom();
        }

    }
}



void ROOTSCOPE::Set_init_fit_value( double range1, double range2,
                                      double &h, double &c, double &sigma )
{

    // only for the click-selected pad
    // passing by reference to determine initial h, c, and sigma values.

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


void ROOTSCOPE::Fit_Gaussian() {



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

        //fTF1_gaus_bg->SetParLimits  ( 3, init_c*0.95, init_c*1.05);

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
            Form( "\nCenter = %5.3f, Area/cmp = %5.2f, FWHM = %5.2f, Chisqr/N = %5.2f (cmp=%d)",
            fitted_c, area/fCmp, FWHM, chisqr, fCmp ) <<  endl;
        fText_viewer->ShowBottom();

         //cout  <<  Form( "\ntest =>h= %6.2f, c = %6.2f, sigma = %6.2f\n", fitted_h, fitted_c, fitted_sigma) <<  endl;
    }

}



void ROOTSCOPE::Fit_Background_pol1() {


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



void ROOTSCOPE::Fit_Background() {

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


void ROOTSCOPE::Set_Background() {

    fBG_linear = 0; // reset

    // pop up a dialog for user to set bg value.
    new Dlg_Set_Background( gClient->GetRoot(), this, 10, 10, &fBG_const );

    fTF1_bg_const->SetParameter(0, fBG_const); // set the 0th par = fBG_const

    To_Draw_bg();
}


void ROOTSCOPE::To_Draw_bg() {

    /* only plot the bg level line on click-selected pad. */

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
        *fText_viewer << Form( "\nbg= %6.2f + %6.2f * x", fBG_const, fBG_linear ) <<  endl;
    }


    c1->GetPad(fPadActive)->Update();

    fText_viewer->ShowBottom();
}



void ROOTSCOPE::Fit_Double_Gaussian() {

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
        fTF1_double_gaus_bg->SetParLimits ( 2, 0, init_h1*2);// height1
        fTF1_double_gaus_bg->SetParLimits ( 3, xMin, xMax ); // center1
        fTF1_double_gaus_bg->SetParLimits ( 4, init_sigma1*0.01, init_sigma1*3 );
        fTF1_double_gaus_bg->SetParLimits ( 5, 0, init_h1*2); // height2
        fTF1_double_gaus_bg->SetParLimits ( 6, xMin, xMax );  // center2
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
        fTF1_gaus_bg->SetLineWidth(1);
        fTF1_gaus_bg->SetLineStyle(2);
        fTF1_gaus_bg->Draw("same");

        fTF1_gaus_bg_2
        ->SetParameters( fBG_const, fBG_linear, fitted_h2, fitted_c2, fitted_sigma2 );
        fTF1_gaus_bg_2->SetRange( lower_limit, upper_limit );
        fTF1_gaus_bg_2->SetLineColor(kGreen);
        fTF1_gaus_bg_2->SetLineWidth(1);
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


void ROOTSCOPE::Expand( ) {


     // organize the range
    float xMax = Get_Max_range( fXrange_pick1, fXrange_pick2 );
    float xMin = Get_Min_range( fXrange_pick1, fXrange_pick2 );

    // note: fIsSameDim = true when
    // we have (a) mulitple pads display and (b) all pads have same x dimensions.


    // single pad or multiple pads with different dimensions.
    if( (xMax != xMin) && !fIsSameDim )
    {
        // only expand the histo in the selected pad.
        histo->GetXaxis()->SetRangeUser( xMin, xMax);
        histo->Draw();
        c1->Update();

    }


    // multiple pads with same dimensiton
    if( (xMax != xMin) && fIsSameDim )
    {
        // expand all the histos.
        for( int i=1; i <= fPadTotalN; i++ ) {
            c1->cd(i);
            Get_histo_by_padnum(i)->GetXaxis()->SetRangeUser( xMin, xMax);
            Get_histo_by_padnum(i)->Draw();
        }
        c1->Update();
        c1->cd( fPadActive );
    }

}



void ROOTSCOPE::Expand_dlg( ) {

    /* pop up a dialog for user to input the range for expanding. */

    float xMax = Get_Max_range( fXrange_pick1, fXrange_pick2 );
    float xMin = Get_Min_range( fXrange_pick1, fXrange_pick2 );

    // two absolute limits.
    float lower_limit = histo->GetXaxis()->GetXmin();
    float upper_limit = histo->GetXaxis()->GetXmax();

    new Dlg_Set_XRange(gClient->GetRoot(), 10, 10,
                        lower_limit, upper_limit,
                         &xMin , &xMax );



    // single pad or multiple pads with different dimensions.
    if( (xMax != xMin) && !fIsSameDim )
    {
        // only expand the histo in the selected pad.
        histo->GetXaxis()->SetRangeUser( xMin, xMax);
        histo->Draw();
        c1->cd( fPadActive );
        c1->Update();
    }


    // multiple pads with same dimensiton
    if( (xMax != xMin) && fIsSameDim )
    {
        // expand all the histos.
        for( int i=1; i <= fPadTotalN; i++ ) {
            c1->cd(i);
            Get_histo_by_padnum(i)->GetXaxis()->SetRangeUser( xMin, xMax);
            Get_histo_by_padnum(i)->Draw();
        }
        c1->Update();
        c1->cd( fPadActive );
    }


    // update markers
    fXrange_pick1 = xMin;
    fXrange_pick2 = xMax;

}


void ROOTSCOPE::Unexpand( ) {

    // single pad or multiple pads with different dimensions.
    if( !fIsSameDim )
    {
        // only unexpand the histo in the selected pad.
        histo-> GetXaxis()-> UnZoom();
        histo-> GetYaxis()-> UnZoom();
        c1->cd( fPadActive );
        histo->Draw();
        c1->Update();
    }


    // multiple pads with same dimensiton
    if( fIsSameDim )
    {
        // unexpand all the histos.
        for( int i=1; i <= fPadTotalN; i++ ) {
            c1->cd(i);
            Get_histo_by_padnum(i)->GetXaxis()->UnZoom();
            Get_histo_by_padnum(i)->GetYaxis()->UnZoom();
            Get_histo_by_padnum(i)->Draw();
        }
        c1->Update();
        c1->cd( fPadActive );
    }

    fXrange_pick1 = Get_histo_by_padnum( fPadActive )->GetXaxis()->GetXmin();
    fXrange_pick2 = Get_histo_by_padnum( fPadActive )->GetXaxis()->GetXmax();

}


void ROOTSCOPE::To_show_find_peaks_dlg() {


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



void ROOTSCOPE::To_find_peaks() {


    //
    // clean up previous polymarkers or text labels (if exists)
    //
    TList*  all_items;
    Int_t objN;
    TVirtualPad* currentPad = c1->GetPad( fPadActive );
    c1->cd( fPadActive );

    all_items = (TList*) currentPad->GetListOfPrimitives();
    objN = all_items->GetSize();
    for(int i=0; i< objN; i++){
        TObject* element = all_items-> At( i );
        TPolyMarker* obj = dynamic_cast<TPolyMarker*> ( element );
        if ( obj == nullptr ) continue;
        delete obj;}
    all_items = (TList*) currentPad->GetListOfPrimitives();
    objN = all_items->GetSize();
    for(int i=0; i< objN; i++){
        TObject* element = all_items-> At( i );
        TText* obj = dynamic_cast<TText*> ( element );
        if ( obj == nullptr ) continue;
        delete obj; }




    // to use TSpectrum to get peaks.( Max peaks = 100 )
    const Int_t     nfoundMax = 100;

    Int_t nfound = peakFinder -> Search( histo, fSigma_guess, "goff", fThreshold );
    Float_t    peak_pos[ nfoundMax ] = {0};
    Float_t    peak_y[ nfoundMax ] = {0};

    //
    //  it seems like once a single pointer changes the address it points to,
    //  canvas will plot both current and the one before we change the address.
    //  we can thus use this property to create polymarker/text for multiple pads
    //  with just one single polymarker/text pointer
    //
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
        TString s = Form(fPrecisionFormat.Data(), peak_pos[i] );
        tmp_text->SetText( peak_pos[i], peak_y[i], s.Data() );
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
    TObjLink *lnk = fTexts->FirstLink();
    while (lnk) { lnk->GetObject()->Draw( "same" ); lnk = lnk->Next(); }
    fPolyMarkers->Draw( );


    c1->Update();
}



void ROOTSCOPE::Rebin_compress() {

    // store the current view range
    int bin_local_1 = histo->GetXaxis()->GetFirst();
    int bin_local_2 = histo->GetXaxis()->GetLast();
    float x1 = histo->GetXaxis()->GetBinCenter( bin_local_1 );
    float x2 = histo->GetXaxis()->GetBinCenter( bin_local_2 );
    fCmp += 1;



     // single pad or multiple pads with different dimensions.
    if( !fIsSameDim ) {

        // reset histo to its original condition.
        Int_t idx;
        if( fPadTotalN == 1 ){ idx = 0; }
        else if ( fPadTotalN > 1 ){ idx = fPadActive - 1; }
        histo_backup = histos_backup.at( fOpen_list.at(idx) );
        histo_backup->Copy( *histo  );

        histo->Rebin( fCmp );
        histo->GetXaxis()->SetRangeUser( x1, x2 );
        c1->cd( fPadActive);
        histo->Draw();
        c1->Update();
    }




    // multiple pads with same dimensiton
    if( fIsSameDim ){

        // reset histo to its original condition.
        for( int i =0; i< histos.size(); i++ ){
            histos_backup.at(i) -> Copy( *histos.at(i) );   }


        for( int i=1; i <= fPadTotalN; i++ ) {

            c1->cd(i);
            Get_histo_by_padnum(i)->Rebin( fCmp );
            Get_histo_by_padnum(i)->GetXaxis()->SetRangeUser( x1, x2 );
            Get_histo_by_padnum(i)->Draw(); }

        c1->Update();
        c1->cd( fPadActive );

    }


    if( fPadTotalN == 1 ) {  histo = histos.at( fOpen_list.at(0) ); }
    if( fPadTotalN > 1 ) { Int_t hIdx = fOpen_list.at( fPadActive-1 );
                           histo = histos.at( hIdx );  }

    *fText_viewer << Form( "\n rebin by %d", fCmp ) <<  endl;
    fText_viewer->ShowBottom();



}


void ROOTSCOPE::Rebin_uncompress() {


    // store the current view range
    int bin_local_1 = histo->GetXaxis()->GetFirst();
    int bin_local_2 = histo->GetXaxis()->GetLast();
    float x1 = histo->GetXaxis()->GetBinCenter( bin_local_1 );
    float x2 = histo->GetXaxis()->GetBinCenter( bin_local_2 );



    if( fCmp > 1 ) {
        fCmp -= 1 ;
    } else {
        fCmp = 1;
    }



     // single pad or multiple pads with different dimensions.
    if( !fIsSameDim )
    {

        // reset histo to its original condition.
        Int_t idx;
        if( fPadTotalN == 1 ){ idx = 0; }
        else if ( fPadTotalN > 1 ){ idx = fPadActive - 1; }
        histo_backup = histos_backup.at( fOpen_list.at(idx) );
        histo_backup->Copy( *histo  );


        histo->Rebin( fCmp );
        histo->GetXaxis()->SetRangeUser( x1, x2 );
        c1->cd( fPadActive);
        histo->Draw();
        c1->Update();
    }




    // multiple pads with same dimensiton
    if( fIsSameDim )
    {

         // reset histo to its original condition.
         for( int i =0; i< histos.size(); i++ ){
            histos_backup.at(i) -> Copy( *histos.at(i) ); }



        for( int i=1; i <= fPadTotalN; i++ ) {
            c1->cd(i);
            Get_histo_by_padnum(i)->Rebin( fCmp );
            Get_histo_by_padnum(i)->GetXaxis()->SetRangeUser( x1, x2 );
            Get_histo_by_padnum(i)->Draw(); }

        c1->Update();
        c1->cd( fPadActive );
    }



    *fText_viewer << Form( "\n rebin by %d", fCmp ) <<  endl;
    fText_viewer->ShowBottom();

}




void ROOTSCOPE::Go_half_left( ) {


    // this function is only effective after we expend the histo


     // local
    int bin_local_1 = histo->GetXaxis()->GetFirst();
    int bin_local_2 = histo->GetXaxis()->GetLast();
    float xMin = histo->GetXaxis()->GetBinCenter( bin_local_1 );
    float xMax = histo->GetXaxis()->GetBinCenter( bin_local_2 );

    // get the absolute limits ( not local view )
    float lower_limit = histo->GetXaxis()->GetXmin();
    float upper_limit = histo->GetXaxis()->GetXmax();

    if( xMin == lower_limit && xMax == upper_limit ){

        // user dones't expand,
        // so we should do noting.
    }
    else {


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


         // single pad or multiple pads with different dimensions.
        if( !fIsSameDim )
        {
            // only shift the histo in the selected pad.
            histo->GetXaxis()->SetRangeUser( next_xL, next_xR);
            histo->Draw();
            c1->Update();
        }


        // multiple pads with same dimensiton
        if( fIsSameDim )
        {
            // shift all the histos.
            for( int i=1; i <= fPadTotalN; i++ ) {
                c1->cd(i);
                Get_histo_by_padnum(i)->GetXaxis()->SetRangeUser( next_xL, next_xR);
                Get_histo_by_padnum(i)->Draw();
            }
            c1->Update();
            c1->cd( fPadActive );
        }


        // in overlap mode
        if( fOverlapMode ){
            TH1* h1 = (TH1*) histos.at( fOverlaph1Idx );
            TH1* h2 = (TH1*) histos.at( fOverlaph2Idx );

            h1->GetXaxis()->SetRangeUser( next_xL, next_xR);
            h2->GetXaxis()->SetRangeUser( next_xL, next_xR);
            c1->Update();
        }


        // update the makers, important!!
        fXrange_pick1 = next_xL;
        fXrange_pick2 = next_xR;


    }



}



void ROOTSCOPE::Go_half_right( ) {

    // this function is only effective after we expend the histo

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



        // single pad or multiple pads with different dimensions.
        if( !fIsSameDim )
        {
            // only shift the histo in the selected pad.
            histo->GetXaxis()->SetRangeUser( next_xL, next_xR);
            histo->Draw();
            c1->Update();
        }


        // multiple pads with same dimensiton
        if( fIsSameDim )
        {
            // shift all the histos.
            for( int i=1; i <= fPadTotalN; i++ ) {
                c1->cd(i);
                Get_histo_by_padnum(i)->GetXaxis()->SetRangeUser( next_xL, next_xR);
                Get_histo_by_padnum(i)->Draw();
            }
            c1->Update();
            c1->cd( fPadActive );
        }


        // in overlap mode
        if( fOverlapMode ){
            TH1* h1 = (TH1*) histos.at( fOverlaph1Idx );
            TH1* h2 = (TH1*) histos.at( fOverlaph2Idx );

            h1->GetXaxis()->SetRangeUser( next_xL, next_xR);
            h2->GetXaxis()->SetRangeUser( next_xL, next_xR);
            c1->Update();
        }

         // update the makers, important!!
        fXrange_pick1 = next_xL;
        fXrange_pick2 = next_xR;



    }


}



void ROOTSCOPE::Set_histo_title() {

    /* only apply for the click-selected pad. */


    TString h_title = histo->GetTitle();

    // pop up a dialog for user to change histo title.
    new Dlg_Set_hTitle( gClient->GetRoot(), this, 10, 10,
                        &h_title );

    histo->SetTitle( h_title.Data() );
    To_backup_histos(); // here we will backup all the atttributes.

    c1->Update();

}



float ROOTSCOPE::Get_Max_range( float x1, float x2) {
    if ( x1 > x2 ) { return x1; }
    else { return x2; }
}


float ROOTSCOPE::Get_Min_range( float x1, float x2) {
    if ( x1 < x2 ) { return x1; }
    else { return x2; }
}


void ROOTSCOPE::Set_Xrange_pick( float x) {

    if  ( x != fXrange_pick1 )
    {
        fXrange_pick2 = fXrange_pick1;
        fXrange_pick1 = x;
    }
}


void ROOTSCOPE::To_backup_histos() {

    // to clone the histos to histos_backup

    histos_backup.clear();

    int histo_size = histos.size();

    if( histo_size != 0 )
    {

        for( int i=0; i<histo_size; i++ )
        {
            histos_backup.
                push_back( (TH1*) histos.at(i)->Clone()  );
        }
    }

}




void ROOTSCOPE::To_display_histos( bool showMSG  ){

    /*  Here we will set up fPadTotalN and fIsSameDim
        and call c1->Update()
    */

    //  note: fOpen_list stores the idx of histogram
    //  which we want to open.
    fPadTotalN = fOpen_list.size();
    c1->Clear();




    //----------------------------------to place histogram on the canvas
    if( fPadTotalN == 1 ) {

        c1->SetMargin( 0.05, 0.05, 0.1, 0.1);
        c1->cd();
        histos.at( fOpen_list.at(0) )->Draw();

        // in order to skip a mouse clcik to select histo,
        // we have to link histo and set fIsTH2_inPad = false.
        fIsTH2_inPad = false;
        fIsSameDim = false; // for single pad, it affects expend, rebin ...
        fPadActive = 0;     // single pad
        fPadTotalN = 1;
        histo = histos.at(  fOpen_list.at(0) );



    }
    else if ( fPadTotalN > 1 ) {

        // before Divide(). Margin is necessary !!
        // left, right, bottom, top
        c1->SetMargin( 0.05, 0.05, 0.1, 0.1);

        // create multiple pads.
        c1->Divide(1, fPadTotalN, 0, 0 );  // xmarign=0, ymargin=0

        for( int i=0; i<fPadTotalN; i++ )
        {

            c1->cd( i+1 ); // pad idx starts from 1 ( not 0 )!!!

            histos.at(  fOpen_list.at(i) )->Draw();

        }





        //check whether we have the same dimensions

        float* histo_xmax = new float [fPadTotalN];
        float* histo_xmin = new float [fPadTotalN];

        for( int i = 0; i< fPadTotalN; i++) {
            histo_xmax[i] = histos.at( fOpen_list.at(i) )->GetXaxis()->GetXmax();
            histo_xmin[i] = histos.at( fOpen_list.at(i) )->GetXaxis()->GetXmin();
        }

        bool isSame = true;
        for( int j = 0; j< fPadTotalN; j++) {
        for( int i = 0; i< fPadTotalN; i++) {
            if ( histo_xmax[i] != histo_xmax[j] )  isSame = false;
            if ( histo_xmin[i] != histo_xmin[j] )  isSame = false;
        }}


        delete [] histo_xmax;
        delete [] histo_xmin ;


        //  we need this condition, since at the first time
        //  from 2d to 1d histos, fPadActive = 0
        histo = histos.at(  fOpen_list.at( 0 ) );
        fPadActive = 1 ;
        fIsSameDim   = isSame;
        fIsTH2_inPad = false;

    }


    if ( showMSG ){
        TString message = "open spec : ";

        for( Int_t i = 0; i< fOpen_list.size(); i++ ){
            if( i != (fOpen_list.size()-1) ) {
                message += Form("%d, ", fOpen_list.at(i) +1 );
            }else {
                message += Form("%d.", fOpen_list.at(i) +1 );
            }
        }


        *fText_viewer << Form("%s\n", message.Data() ) <<  flush;
        fText_viewer->ShowBottom();
    }


    c1->Update();

}


void ROOTSCOPE::To_delete_histo_inSelectedPad() {

    // we remove the histogram displayed on the selected pad.
    // and it also be removed from histos<TH1*>.
    // we replace the next histogram to the current pad.


    int idx; // the histo idx we will remove.



    // Step 1: retreive the histo index we will remove.

    if( fPadTotalN == 1) { idx = fOpen_list[ 0 ]; }  // single pad case

    else if ( fPadTotalN > 1) { idx = fOpen_list[ fPadActive -1 ]; } // multiple pads case


    /*  // for checking
        for (Int_t i = 0; i < fOpen_list.size(); i++ ) {
        printf( " before erase,  open_list[%d] = %d, histos.size() = %d, padactive = %d \n",
        i, fOpen_list.at(i), histos.size(), fPadActive ); }
    */


    // Step 2: make sure we have at lease 1 histogram after erase()
    if( histos.size() >= 2 ) {
        histos.erase( histos.begin() + idx );
        To_backup_histos();
         *fText_viewer << Form("remove spec at %d", idx + 1 )  <<  endl;
        fText_viewer->ShowBottom();

    }


    /*  // for checking
        for (Int_t i = 0; i < fOpen_list.size(); i++ ) {
        printf( " after erase, open_list[%d] = %d, histos.size() = %d, padactive = %d \n",
        i, fOpen_list.at(i), histos.size(), fPadActive ); }
    */


    // Step 3: we have to figure out how to map the orginal histogram index
    /*
                    x
     histos   = 0 1 2 3 4 5
     openlist = 1 0 3
     4 -> 3 ( shift )
     5 -> 4 ( shift )
    */


    for( Int_t i = 0; i < fOpen_list.size(); i++ )
    {
        if( fOpen_list.at(i) < idx )
        {
            // we don't need to change.
        }
        else if ( fOpen_list.at(i) == idx )
        {

             // we only change when we delete the last histo
             if(idx == histos.size() ) {  fOpen_list.at(i)--; }

        }
        else if ( fOpen_list.at(i) > idx )
        {
            //  adjust by one.
            fOpen_list.at(i)--;
        }


    }



     /*// for checking
        for ( Int_t i = 0; i < fOpen_list.size(); i++ ) {
        printf( " after modify, open_list[%d] = %d, histos.size() = %d, padactive = %d \n",
        i, fOpen_list.at(i), histos.size(), fPadActive );}
    */




    To_display_histos( 0 );

}



void ROOTSCOPE::To_Change_histo_x_range() {

    /* pop up a dialog for user to input the range for changing histogram x range. */


    float xlow_select;
    float xup_select;

    // to reset the compress before we change the x range for histo.
    Int_t idx;
    if( fPadTotalN == 1 ){ idx = 0; }
    else if ( fPadTotalN > 1 ){ idx = fPadActive - 1; }
    Int_t histoIdx = fOpen_list.at(idx); // histogram index.
    histo_backup = histos_backup.at( histoIdx );
    histo_backup->Copy( *histo  );

    // to gather the information of the original histo.
    float xlow = histo->GetXaxis()->GetXmin();
    float xup  = histo->GetXaxis()->GetXmax();
    int binx1  = histo->GetXaxis()-> FindBin(xlow);
    int binx2  = histo->GetXaxis()-> FindBin(xup);
    float binWidth   =histo->GetXaxis()->GetBinWidth(1);


    // let user to select new ranges.
    new Dlg_Change_histo_X_Range(gClient->GetRoot(), 10, 10,
                        xlow, xup,
                        &xlow_select , &xup_select );



    if( (xlow_select == xlow) && (xup_select == xup) )
    {   /* do nothing, because we don't change x range.*/
        histo->Rebin( fCmp );

    }
    else{

        // do the calculations, get the xlow2 and xup2 but keeping the same bin width.
        Calculate_xlow2_xup2* cal
        = new Calculate_xlow2_xup2(xlow, xup, xlow_select, xup_select, binWidth);

        Int_t     binN2 = cal->Get_binN2();
        Float_t   xup2  = cal->Get_xup2();
        Float_t   xlow2 = cal->Get_xlow2();
        delete cal;

        // create the new histo, and fill the data.
        TH1F* histoNew = new TH1F( "histoNew", "histoNew", binN2, xlow2, xup2 );
        histoNew->SetTitle( histo->GetTitle() );
        // histoNew->SetTitle( Form("new copy %s", histo->GetTitle()) ); // for test
        histoNew->SetName( Form("%s_%f", histo->GetTitle() , gRandom->Uniform()  ) );

        for( Int_t i =binx1; i<=binx2; i++ ){
                float xxx = histo->GetXaxis()->GetBinCenter( i );
                float yyy = histo->GetBinContent( i );

                int   bin_for_histoNew = histoNew->GetXaxis()->FindBin( xxx );
                histoNew->SetBinContent( bin_for_histoNew, yyy);
        }


        // insert the new histo, and then erase the old one.
        histos.insert( histos.begin() + histoIdx, (TH1*) histoNew->Clone() );
        histo = histos.at( histoIdx );
        histos.erase( histos.begin() + histoIdx + 1 );


        To_backup_histos();
        histo->Rebin( fCmp );

        To_display_histos(0);
        c1->Update();
        delete histoNew;
    }



}


void ROOTSCOPE::_doing_overloap( TH1* h1, TH1* h2,
                                  Int_t h1Idx, Int_t h2Idx, Bool_t ShowMsg ) {

    int bin_local_1 = h1->GetXaxis()->GetFirst();
    int bin_local_2 = h1->GetXaxis()->GetLast();
    float xMin = h1->GetXaxis()->GetBinCenter( bin_local_1 );
    float xMax = h1->GetXaxis()->GetBinCenter( bin_local_2 );

    //calculate the common x range for h1 and h2
    Calculate_common_x_range* mycal
    = new Calculate_common_x_range( h1, h2, xMin, xMax );

    Float_t x_common_low = mycal->Get_lower_bound();
    Float_t x_common_up  = mycal->Get_upper_bound();
    Bool_t  is_same_dim  = mycal->Is_same_dim();
    delete mycal;


    // reset to single pad.
    c1->Clear();
    c1->cd(0);

    h1->Draw();
    h1->SetTitle( Form("To overlap spec%02d with spec%02d", h1Idx+1 , h2Idx+1 ) );
    h1->GetXaxis()->SetRangeUser( x_common_low, x_common_up);
    Float_t h1Uymax = h1->GetMaximum() / 0.9;   // User y max
    h1->GetYaxis()->SetRangeUser( 0, h1Uymax );

    // for fill style setting.
    if( fH1_fillstyle == 1 ) {
        h1->SetFillColor( h1->GetLineColor() );
        h1->SetFillStyle( 3004 );
    } else if ( fH1_fillstyle == 0 ) { h1->SetFillStyle( 0 ); }

    c1->Update(); // the update here is necessary.
    //Note: GetMaximum() will return the local Max y value.

    h2->GetXaxis()->SetRangeUser( x_common_low, x_common_up);
    Float_t h2Uymax = h2->GetMaximum() / 0.9 ; // User y max
    Float_t h2Uymin = 0;
    Float_t scale    = c1->GetPad(0)->GetUymax()/h2Uymax;

    h2->SetLineColor( kRed );

    // for fill style setting.
    if( fH1_fillstyle == 1 ) {
        h2->SetFillColor(  h2->GetLineColor() );
        h2->SetFillStyle( 3004 );
    } else if ( fH1_fillstyle == 0 ) {  h2->SetFillStyle( 0 ); }



    h2->Scale(scale);
    h2->Draw("same");

    TVirtualPad* curPad = c1->GetPad(0);
    faxis
    = new TGaxis(   curPad->GetUxmax(), curPad->GetUymin(),
                    curPad->GetUxmax(), curPad->GetUymax(),
                    h2Uymin, h2Uymax, 510, "+L" );
    //Number of divisions = 510,
    // '+': tick marks are drawn on Positive side
    // 'L': labels are Left adjusted on tick mark.

    faxis->SetLineColor(kRed);   // the axis line color
    faxis->SetLabelColor(kRed);  // the tick mark color
    faxis->Draw();

    c1->Update();
    // In case that user press key to invoke command like rebin,
    // in default, I set the commands to act to a single pad
    // for the fisrt histogram we open when doing overlapping.
    // we need this to avoid crash.
    histo = histos[ h1Idx ];
    fOpen_list.clear();
    fOpen_list.push_back( h1Idx  );
    Single_pad_setting(); // single pad


    //warn user if we don't have the same dimensions

    if( is_same_dim  ) {

        if( ShowMsg ) {
            *fText_viewer
            << Form( "\noverlap x range from %.3f to %.3f.", x_common_low, x_common_up )
            <<  endl;
        }

    }
    else {
        *fText_viewer
          << Form( "\nWARNING WE DON'T HAVE SAME DIMENSIONS FOR THESE TWO HISTOGRAMS.\nHere we overlap the common x range from %.3f to %.3f.", x_common_low, x_common_up )
        <<  endl;


    }
    fText_viewer->ShowBottom();


}



void ROOTSCOPE::To_switch_overlap() {

    /*
        we only overlap two pads, and use key "c" to
        toggle the overlap mode.
    */

    Int_t activePadBackup = fPadActive;

    if( fSwitch_overlap%2==0 ) { fSwitch_overlap = 0; }

    if ( fPadTotalN == 2 && fSwitch_overlap == 1 ) {

        fOverlaph1Idx = fOpen_list[0];
        fOverlaph2Idx = fOpen_list[1];

        // overlap the present two histograms.
        // Note: Clone works, but Copy not.
        TH1* h1 = (TH1*) histos.at( fOpen_list[0] )->Clone();
        TH1* h2 = (TH1*) histos.at( fOpen_list[1] )->Clone();

        _doing_overloap( h1, h2, fOverlaph1Idx, fOverlaph2Idx, 0 );

        fOverlapMode = true ;
        fPadActive = activePadBackup;
        // _doing_overlap function will change fPadActive,
        // so I think it is a good idea to restore it.


    }


    if( fOverlapMode &&  fSwitch_overlap != 1) {


        // we want the next time to enter this loop, so
        // I add a condition of "Switch_overlap != 1"
        // fOverlapMode is to ensure that we indeed go through the first loop.


        fOpen_list.clear();
        fOpen_list.push_back( fOverlaph1Idx );
        fOpen_list.push_back( fOverlaph2Idx );
        To_display_histos( 0 );
        fOverlapMode = false;

        fPadActive = activePadBackup;
        Int_t hIdx = fOpen_list.at( fPadActive-1 );
        histo = histos.at( hIdx );
        // use activePadBackup to allow user to continue operate on the same pad.

    }


    fSwitch_overlap ++;

}



void ROOTSCOPE::To_overlap_histos() {

    /* currently only for two histograms overlap */

    TString         list = "";
    TString         tmp;
    int             histo_idx;
    vector<int>     overlapHistoIndx;
    vector<TH1*>    overlapHisto;     // Note: overlapHisto will be recreated.

    // pop up a dialog to let user to select the histogram(s) to overlap.
    new Dlg_Display_histos( gClient->GetRoot(), 500, 400, &list, histos  );



    if( list.CompareTo("") != 0 )
    {
        // separate by " "
        TObjArray* tmp_array = list.Tokenize(" ");

        for( int i=0; i< tmp_array->GetEntries(); i++){

            // retrieve and convert
            tmp = ( (TObjString*)tmp_array->At(i) )->GetString();
            histo_idx = tmp.Atoi() - 1;

            // validation.
            if ( histo_idx >=0 && histo_idx < histos.size() )
            {
                overlapHistoIndx.push_back( tmp.Atoi() - 1 );
                overlapHisto.push_back( (TH1*)  histos[histo_idx] ->Clone()  );
            }
        }
     } //---------end of if





    Int_t   N = overlapHistoIndx.size();

    if( N == 2 ) {

        TH1* h1 = overlapHisto[0];
        TH1* h2 = overlapHisto[1];
        Int_t h1Idx = overlapHistoIndx[0];
        Int_t h2Idx = overlapHistoIndx[1];

        _doing_overloap( h1, h2, h1Idx, h2Idx, 1 );


    } //============ end of if( N == 2 )



}


void ROOTSCOPE::To_switch_histo( bool toIncrease ) {

    /*  This method links to the key reponse by arrow keys,
        which is used to switch the histogram.

        I disable the switching in the overlap mode.

        Reminder:
        vector<int> fOpen_list is for the index of histos that we displayed
        fPadActive is the pad number for the selected pad.
        when single pad displayed. fPadActive = 0
        when N pads displayed, fPadActive from 1 to N.
        !!!!  Pad index starts from 1
    */


    Int_t activePadBackup = fPadActive;


    bool hasChange = false;

    if( fPadTotalN == 1 && fOverlapMode == 0 ) // disable switching in overlap mode.
    {

        // increase the index by 1
        if( toIncrease &&   fOpen_list[0] < (histos.size()-1)    )
        {
            fOpen_list[0] ++;   hasChange = true;

        }

        if( !toIncrease && fOpen_list[0] >0 )
        {
            fOpen_list[0] --;   hasChange = true;

        }
        *fText_viewer
        << Form("switch to spec#%d",fOpen_list[0] + 1 )  <<  endl;
        fText_viewer->ShowBottom();

    }

    if( fPadTotalN > 1  )
    {

        int idx = fPadActive-1;
        if( toIncrease &&   fOpen_list[ idx ] < (histos.size()-1)    )
        {
            fOpen_list[ idx ] ++;   hasChange = true;

        }

        if( !toIncrease && fOpen_list[ idx ] >0 )
        {
            fOpen_list[ idx ] --;   hasChange = true;

        }

        *fText_viewer << Form("switch to spec no. %d",fOpen_list[ idx ] + 1 )  <<  endl;
        fText_viewer->ShowBottom();

    }


    bool showMSG;
    if( hasChange) To_display_histos( showMSG = false );

    if( fPadTotalN > 1 ) {

        fPadActive = activePadBackup;
        Int_t hIdx = fOpen_list.at( fPadActive-1 );
        histo = histos.at( hIdx );
    }


}


void ROOTSCOPE::To_Back_to_disply_histos() {

    if( fOpen_list.size() != 0 ) To_display_histos(1);

}


void ROOTSCOPE::To_show_histo_operation_dlg() {

    /* pop a dialog for user to input expression, ex:
        99 = 1 + 2 ( then we do histos[0] + histos[1] )
    */

    TString histo_list = "";
    TString output_message = "";


    new Dlg_Operation_histos( gClient->GetRoot(), 500, 400,
                             &histo_list, &histos, &output_message );

    *fText_viewer << output_message.Data()  <<  endl;
    fText_viewer->ShowBottom();

    // open the one we just operated.
    fOpen_list.clear();

    fOpen_list.push_back( histo_list.Atoi() );
    To_backup_histos();
    To_display_histos(1);
}


void ROOTSCOPE::To_show_histo_dlg() {

    TString list = "";

    for( Int_t i=0; i<fOpen_list.size(); i++) { list += Form("%d ", fOpen_list[i]+1); }


    TString tmp;
    int histo_idx;
    // pop up a dialog to let user to select the histogram(s) to display.
    new Dlg_Display_histos( gClient->GetRoot(), 500, 400, &list, histos  );


    if( list.CompareTo("") != 0 ) {

        // when we have an update, list will have an non empty string. (like "1 5 2")
        // and we will update our fOpen_list.

        fOpen_list.clear();

        // Parse the list, and then assign it to vector<int> fOpen_list
        TObjArray* tmp_array = list.Tokenize(" "); // separate by " " space..

        for( int i=0; i< tmp_array->GetEntries(); i++){

            // retrieve and convert
            tmp = ( (TObjString*)tmp_array->At(i) )->GetString();


            histo_idx = tmp.Atoi() - 1;

            // validation.
            if ( histo_idx >=0 && histo_idx < histos.size() ) {
                fOpen_list.push_back( tmp.Atoi() - 1 );
            }

        }
    }


    if( fOpen_list.size() != 0 ) To_display_histos(1);

}


void ROOTSCOPE::To_show_AddSub_gate_dlg( ) {


    Float_t  addGate1 = fXrange_pick1;
    Float_t  addGate2 = fXrange_pick2;
    Float_t  subGate1 = fGeneral_pick1;
    Float_t  subGate2 = fGeneral_pick2;
    TString  projDirection="";
    Bool_t   do_prject = false;

    Dlg_projection_subtractMode* testObj
    = new Dlg_projection_subtractMode
    ( gClient->GetRoot(),   &addGate1, &addGate2,
                            &subGate1, &subGate2,
                            &projDirection, &do_prject );

     if( do_prject ){

        projDirection = projDirection.Strip ( TString::kBoth, ' ' );
        projDirection.ToLower();
        Bool_t gating_on_x_proj_to_y;

        if(  projDirection == "x" ) { gating_on_x_proj_to_y = true;  }
        if ( projDirection == "y" ) { gating_on_x_proj_to_y = false;  }

        Calculate_addSub_bg* gateManager =
        new Calculate_addSub_bg(histo2d, addGate1, addGate2,
                                        subGate1, subGate2,
                                        gating_on_x_proj_to_y );

        //
        //  To append the new histo to histos
        //
        TH1*    pHisto;
        TString pHisto_title;
        pHisto = gateManager->get_histogram();
        pHisto_title = Form("gate_X_%.1f_%.1f_bg=%.1f_%.1f",
                        addGate1, addGate2, subGate1, subGate2 );
        pHisto->SetName( pHisto_title.Data() );
        pHisto->SetTitle( pHisto_title.Data() );
        histos.push_back( pHisto );
        delete gateManager;

        To_backup_histos();
        fOpen_list.clear();
        fOpen_list.push_back( histos.size() - 1 );  // index for the last one
        To_display_histos(0);

         *fText_viewer <<
        Form("create a histo from gating Y: %.1f - %.1f, bg: %.1f to %.1f, at spec %d\n",
                addGate1, addGate2, subGate1, subGate2, static_cast<int>(histos.size()) ) ;
        fText_viewer->ShowBottom();


    }


}


void ROOTSCOPE::Project_from_1d( bool flagXY ){

    if( fHasTH2 )
    {

        TH1*    pHisto;
        TH1*    pHisto_total;
        TString pHisto_name;  // id for the projected histogram
        TString pHisto_title; // title for the projected histogram
        Int_t n = histos.size() + 1;

        float xMax = TMath::Max( fXrange_pick1, fXrange_pick2 );
        float xMin = TMath::Min( fXrange_pick1, fXrange_pick2 );
        int binx1 = histo2d->GetXaxis()-> FindBin(xMin);
        int binx2 = histo2d->GetXaxis()-> FindBin(xMax);

        float yMax = Get_Max_range( fXrange_pick1, fXrange_pick2 );
        float yMin = Get_Min_range( fXrange_pick1, fXrange_pick2 );
        int biny1 = histo2d->GetYaxis()-> FindBin(yMin);
        int biny2 = histo2d->GetYaxis()-> FindBin(yMax);



        if( flagXY == 1 &&  (xMax != xMin )   ) // gating x axis
        {

            if( fBG_const ==0 && fBG_linear == 0) {

                // gating without any background.
                pHisto_name  = Form("gate_X_%.1f_%.1f", xMin, xMax );
                pHisto_title = Form("gate on X: %.1f - %.1f", xMin, xMax );
                pHisto = histo2d->ProjectionY( pHisto_name.Data(), binx1, binx2 );
                pHisto->SetTitle( pHisto_title.Data() );
                histos.push_back( pHisto );


            }

            if( fBG_const !=0 && fBG_linear == 0 ){

                //gating with fraction bg substraction.
                Calculate_fraction_bg* gateManager
                = new Calculate_fraction_bg( histo2d, xMin, xMax, fBG_const, 1 );

                pHisto = gateManager->get_histogram();
                pHisto_name  = Form("gate_X_%.1f_%.1f(bg=%.1f)", xMin, xMax, fBG_const );
                pHisto_title = Form("gate_X_%.1f_%.1f(bg=%.1f)", xMin, xMax, fBG_const );
                pHisto->SetTitle( pHisto_title.Data() );
                histos.push_back( pHisto );
                delete gateManager;

            }


            *fText_viewer <<
            Form("create a histo from gating X: %.1f - %.1f, at spec %d\n",
                xMin, xMax, static_cast<int>(histos.size()) );
            fText_viewer->ShowBottom();
        }

        if( flagXY == 0 && (xMax != xMin ) ) // gating y axis
        {

            if( fBG_const ==0 && fBG_linear == 0){

                // gating without any background.
                pHisto_name  = Form("gating_Y_%.1f_%.1f", yMin, yMax );
                pHisto_title = Form("gate on Y: %.1f - %.1f", yMin, yMax );
                pHisto = histo2d->ProjectionX( pHisto_name.Data(), biny1, biny2 );
                pHisto->SetTitle( pHisto_title.Data() );
                histos.push_back( pHisto );
            }

            if( fBG_const !=0 && fBG_linear == 0 ){

                //gating with fraction bg substraction.
                 Calculate_fraction_bg* gateManager
                = new Calculate_fraction_bg( histo2d, xMin, xMax, fBG_const, 0 );

                pHisto = gateManager->get_histogram();
                pHisto_name  = Form("gate_Y_%.1f_%.1f(bg=%.1f)", xMin, xMax, fBG_const );
                pHisto_title = Form("gate_Y_%.1f_%.1f(bg=%.1f)", xMin, xMax, fBG_const );
                pHisto->SetTitle( pHisto_title.Data() );
                histos.push_back( pHisto );
                delete gateManager;

            }


            *fText_viewer <<
            Form("create a histo from gating Y: %.1f - %.1f, at spec %d\n",
                yMin, yMax, static_cast<int>(histos.size()) ) ;
            fText_viewer->ShowBottom();
        }



        To_backup_histos();
        fOpen_list.clear();
        fOpen_list.push_back( histos.size() - 1 );  // index for the last one
        To_display_histos(0);


    } else {

        *fText_viewer <<
        Form("we don't have a 2d histogram to do projection yet.\n" ) ;
        fText_viewer->ShowBottom();

    }


}


void ROOTSCOPE::Projection_2d( bool flagXY ){

    /*
        To display the projection from a 2d matrix,
        either gating by x or y.
        the result will be shown in a single pad after projection.
    */

    // organize the range
    float xMax = Get_Max_range( fH2_pickX[0], fH2_pickX[1] );
    float xMin = Get_Min_range( fH2_pickX[0], fH2_pickX[1] );
    float yMax = Get_Max_range( fH2_pickY[0], fH2_pickY[1] );
    float yMin = Get_Min_range( fH2_pickY[0], fH2_pickY[1] );

    int binx1 = histo2d->GetXaxis()-> FindBin(xMin);
    int binx2 = histo2d->GetXaxis()-> FindBin(xMax);
    int biny1 = histo2d->GetYaxis()-> FindBin(yMin);
    int biny2 = histo2d->GetYaxis()-> FindBin(yMax);

    TH1*    pHisto;
    TString pHisto_name;  // id for the projected histogram
    TString pHisto_title; // title for the projected histogram
    Int_t n = histos.size() + 1;

    if( flagXY == 1 ) // gating x axis
    {

        pHisto_name  = Form("gate_X_%.1f_%.1f", xMin, xMax );
        pHisto_title = Form("gate on X: %.1f - %.1f", xMin, xMax );
        pHisto = histo2d->ProjectionY( pHisto_name.Data(), binx1, binx2 );
        pHisto->SetTitle( pHisto_title.Data() );
        histos.push_back( pHisto );
        *fText_viewer <<
        Form("create a histo from gating X: %.1f - %.1f, at spec %d\n",
            xMin, xMax, static_cast<int>(histos.size()) );
        fText_viewer->ShowBottom();
    }

    if( flagXY == 0 ) // gating y axis
    {
        pHisto_name  = Form("gating_Y_%.1f_%.1f", yMin, yMax );
        pHisto_title = Form("gate on Y: %.1f - %.1f", yMin, yMax );
        pHisto = histo2d->ProjectionX( pHisto_name.Data(), biny1, biny2 );
        pHisto->SetTitle( pHisto_title.Data() );
        histos.push_back( pHisto );
        Form("create a histo from gating Y: %.1f - %.1f, at spec %d\n",
            yMin, yMax, static_cast<int>(histos.size()) );
        fText_viewer->ShowBottom();
    }


    To_backup_histos();
    fOpen_list.clear();
    fOpen_list.push_back( histos.size() - 1 );  // index for the last one
    To_display_histos( 0 );



}


void ROOTSCOPE::To_display_histo2d() {

    /* display histo2d */



    if( fHasTH2  ) {
        // make sure we do have TH2 object from input.
        c1->Clear();
        c1->SetMargin( 0.1, 0.1, 0.1, 0.1);
        fIsTH2_inPad = true;
        histo2d->Draw( styleDraw[fH2_style]  );
        c1->Update();
        *fText_viewer << Form("open 2d histogram")  <<  endl;
        fText_viewer->ShowBottom();
    }
    else {

        *fText_viewer << Form("no 2d histogram is set")  <<  endl;
        fText_viewer->ShowBottom();
    }




}


void ROOTSCOPE::SetMarker_2d(Event_t* e){

    TVirtualPad* currentPad = c1->GetClickSelectedPad();

    int        px = e->fX;
    float     xx  = currentPad->AbsPixeltoX(px);
    int     binx  = histo2d->GetXaxis()-> FindBin(xx);
    float   x_pos = histo2d->GetXaxis()-> GetBinCenter( binx );



    int        py = e->fY;
    float     yy  = currentPad->AbsPixeltoY(py);
    int     biny  = histo2d->GetYaxis()-> FindBin(yy);
    float   y_pos = histo2d->GetYaxis()-> GetBinCenter( biny );


    if( fWhich_active_2d == 1) {

        Set_Line( fH2_marker_setA[0],
            x_pos, currentPad->GetUymin(),
            x_pos, currentPad->GetUymax() );  fH2_marker_setA[0]->Draw();

        Set_Line( fH2_marker_setA[1],
            currentPad->GetUxmin(), y_pos,
            currentPad->GetUxmax(), y_pos );  fH2_marker_setA[1]->Draw();

        fWhich_active_2d = 2;

    } else if ( fWhich_active_2d == 2 ) {

        Set_Line( fH2_marker_setB[0],
            x_pos, currentPad->GetUymin(),
            x_pos, currentPad->GetUymax() );  fH2_marker_setB[0]->Draw();

        Set_Line( fH2_marker_setB[1],
            currentPad->GetUxmin(), y_pos,
            currentPad->GetUxmax(), y_pos );  fH2_marker_setB[1]->Draw();

        fWhich_active_2d = 1;
    }


    // assign and update to our pick marker
    if( x_pos != fH2_pickX[0] ) {
        fH2_pickX[1] = fH2_pickX[0]; fH2_pickX[0] = x_pos;  }

     if( y_pos != fH2_pickY[0] ) {
        fH2_pickY[1] = fH2_pickY[0]; fH2_pickY[0] = y_pos;  }

    c1->Update();




    float   counts = histo2d->GetBinContent( binx, biny );
     *fText_viewer << Form("x = %6.2f, y = %6.2f, counts = %6.2f", x_pos, y_pos, counts )  <<  endl;
    fText_viewer->ShowBottom();

}


void ROOTSCOPE::Expand_2d( ) {



     // organize the range
    float xMax = Get_Max_range( fH2_pickX[0], fH2_pickX[1] );
    float xMin = Get_Min_range( fH2_pickX[0], fH2_pickX[1] );

    float yMax = Get_Max_range( fH2_pickY[0], fH2_pickY[1] );
    float yMin = Get_Min_range( fH2_pickY[0], fH2_pickY[1] );




    if( (xMax != xMin) || (yMax != yMin) )
    {

        histo2d->GetXaxis()->SetRangeUser( xMin, xMax);
        histo2d->GetYaxis()->SetRangeUser( yMin, yMax);
        histo2d->Draw( styleDraw[fH2_style]  );
    }

    c1->Update();


}


void ROOTSCOPE::Unexpand_2d( ){

        histo2d-> GetXaxis()-> UnZoom();
        histo2d-> GetYaxis()-> UnZoom();

        float primitive_zmax = histo2d->GetBinContent( histo2d->GetMaximumBin() );
        float primitive_zmin = histo2d->GetBinContent( histo2d->GetMinimumBin() );
        histo2d->SetMaximum( primitive_zmax );
        histo2d->SetMinimum( primitive_zmin );

        histo2d->Draw( styleDraw[fH2_style]  );
        c1->Update();

        // reset pick
        fH2_pickX[0] = histo2d->GetXaxis()->GetXmin();
        fH2_pickX[1] = histo2d->GetXaxis()->GetXmax();
        fH2_pickY[0] = histo2d->GetYaxis()->GetXmin();
        fH2_pickY[1] = histo2d->GetYaxis()->GetXmax();
        // do I need these? we have the same function at Clear_Marker2d()

}


void ROOTSCOPE::Clear_Marker2d() {

    for(int i =0; i<2; i++) {
        Set_Line( fH2_marker_setA[i], 0, 0, 0, 0);  fH2_marker_setA[i]->Draw();
        Set_Line( fH2_marker_setB[i], 0, 0, 0, 0);  fH2_marker_setB[i]->Draw();
    }

    // reset pick
    fH2_pickX[0] = histo2d->GetXaxis()->GetXmin();
    fH2_pickX[1] = histo2d->GetXaxis()->GetXmax();
    fH2_pickY[0] = histo2d->GetYaxis()->GetXmin();
    fH2_pickY[1] = histo2d->GetYaxis()->GetXmax();


    histo2d->Draw( styleDraw[fH2_style] );
    c1->Update();
}


void ROOTSCOPE::Rebin_2d(bool flagXY, bool flag2 ) {

    /*  flagXY is to control which axis we want to deal with
        flagXY = 1 for X axis, flagXY = 0 for Y axis

        flag2 is to control fCmp_2d (compress factor )
        flag2 = 1 for cmp++, flag2 = 0 for cmp--
    */

    // store the current view range
    int bin_local_1;
    int bin_local_2;
    bin_local_1 = histo2d->GetXaxis()->GetFirst();
    bin_local_2 = histo2d->GetXaxis()->GetLast();
    float x1 = histo2d->GetXaxis()->GetBinCenter( bin_local_1 );
    float x2 = histo2d->GetXaxis()->GetBinCenter( bin_local_2 );

    bin_local_1 = histo2d->GetYaxis()->GetFirst();
    bin_local_2 = histo2d->GetYaxis()->GetLast();
    float y1 = histo2d->GetYaxis()->GetBinCenter( bin_local_1 );
    float y2 = histo2d->GetYaxis()->GetBinCenter( bin_local_2 );


    if( flagXY == 1 ) // for X
    {
        if( flag2 == 1 ) { fCmp_2d_X ++; }

        if( flag2 == 0 )
        {
            if( fCmp_2d_X > 1) { fCmp_2d_X --; }
            else{ fCmp_2d_X = 1; }
        }

    }

    if( flagXY == 0 ) // for Y
    {
        if( flag2 == 1 ) { fCmp_2d_Y ++; }

        if( flag2 == 0 )
        {
            if( fCmp_2d_Y > 1) { fCmp_2d_Y --; }
            else{ fCmp_2d_Y = 1; }
        }

    }



    // reset histo2d to its original condition.
    histo2d_backup->Copy( *histo2d  );
    histo2d->Rebin2D( fCmp_2d_X, fCmp_2d_Y );

    histo2d->GetXaxis()->SetRangeUser( x1, x2 );
    histo2d->GetYaxis()->SetRangeUser( y1, y2 );
    histo2d->Draw(  styleDraw[fH2_style] );
    c1->Update();

    *fText_viewer << Form( "\n X rebin by %d, Y rebin by %d", fCmp_2d_X, fCmp_2d_Y ) <<  endl;
    fText_viewer->ShowBottom();

}


void ROOTSCOPE::To_set_logscale(){

    TVirtualPad* currentPad = c1->GetPad( fPadActive );

    fLogScale_style ++;

    TH2* h2temp;
    fIsTH2_inPad = false;
    TList* myList = currentPad->GetListOfPrimitives();
    int objN = myList->GetSize();

    //  to filter out the TH2 object
    for(int i=0; i< objN; i++)
    {
        TObject* element = myList-> At( i );

        h2temp = dynamic_cast<TH2*> ( element );
        if ( h2temp == nullptr ) continue;
        fIsTH2_inPad = true;
    }


    // 1d histo in pad
    if( !fIsTH2_inPad )
    {
        if( fLogScale_style%2 == 0) {
            currentPad->SetLogy(kFALSE);
            fLogScale_style = 0;
        }
        if( fLogScale_style ==1 ) {
            // Note: if user set a customized y range, then set log scale may crash
            // so we need to unzoom the y axis first.
            histo->GetYaxis()->UnZoom();
            currentPad->SetLogy(kTRUE);
        }
    }

    // 2d histo in pad
    if( fIsTH2_inPad ) {

        if( fLogScale_style%4 == 0) {
            currentPad->SetLogz(kFALSE);
            currentPad->SetLogx(kFALSE);
            currentPad->SetLogy(kFALSE);
            fLogScale_style = 0;
        }
        if( fLogScale_style == 1 ) {
            currentPad->SetLogz(kTRUE);
            currentPad->SetLogx(kFALSE);
            currentPad->SetLogy(kFALSE);
        }
        if( fLogScale_style == 2 ) {
            currentPad->SetLogz(kFALSE);
            currentPad->SetLogx(kFALSE);
            currentPad->SetLogy(kTRUE);
        }
        if( fLogScale_style == 3 ) {
            currentPad->SetLogz(kFALSE);
            currentPad->SetLogx(kTRUE);
            currentPad->SetLogy(kFALSE);
        }

    }
    c1->Update();
}


void ROOTSCOPE::To_writeoutFile() {

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



        // check the ending is "xxx.root"
        sizeN = outFileName.Length();
        TString tmp = outFileName(sizeN-5, sizeN-1);
        if( tmp != ".root") { outFileName += ".root"; }


        // write out the data
        TFile outFile( outFileName.Data(), "RECREATE" );
        if( outFile.IsOpen() )
        {
            if( fHasTH2 ) { histo2d->Write();  }
            for( Int_t i = 0; i< histos.size(); i++ ) { histos[i]->Write(); }
            *fText_viewer << Form("write out to %s", outFileName.Data() )<< endl;
            fText_viewer->ShowBottom();
        }

        outFile.Close();
    }

}


void ROOTSCOPE::To_readinFile( ){

    /* we append the 1d histo, but overwrite the 2d histo */

    Bool_t IsChange = Remove_default_empty_histo();


    //-------------------------------------------file dialog
    TString     dir(".");
    TGFileInfo  filieInfo;
    const char *filetypes[] = { "ROOT files",    "*.root",
                                0,               0 };
    filieInfo.fFileTypes = filetypes;
    filieInfo.fIniDir    = StrDup(dir);
    new TGFileDialog( gClient->GetRoot(), this , kFDOpen, &filieInfo);

    TString inFileName  = filieInfo.fFilename;
    *fText_viewer << Form("read in %s", inFileName.Data() )<< endl;

    ReadIn_and_parse* tmpManager
    = new ReadIn_and_parse( &histos, inFileName  );
    // we will update histos here.
    To_backup_histos();


    *fText_viewer <<
    Form("add  %d 1d histogram", tmpManager->Get_loadedTH1number() )<< endl;


    fHasTH2 = tmpManager->Get_hasTH2();


    if( fHasTH2 ) {
        histo2d = tmpManager->Get_TH2();
        *fText_viewer << "Load a 2d histogram"  <<  endl;
        fText_viewer->ShowBottom(); }

    if( IsChange ) {
        // when we have no 1d nor 2d histo initially,
        // after loading, we show the first 1d
        // (2d by To_display_histo2d() will have bugs....
        histo = histos[0];
        fOpen_list.clear();
        fOpen_list.push_back( 0 );
        Single_pad_setting(); // single pad
        To_display_histos( 1 );

    }


    // delete tmpManager;
    //! if we delete tmpManager, we will have memory crash.
    // I think it may be from the we open and read the TFile in the tmpManager,
    // so when we delete it, the memory reference miss?


}


void ROOTSCOPE::To_change_linewidth(){

    fH1_linewidth ++ ;
    if( fH1_linewidth %7 == 0 ) { fH1_linewidth =1; }

    Int_t histoN = histos.size();
    for(Int_t i=0; i<histoN; i++) {
        histos[i]->SetLineWidth(fH1_linewidth  );
        histos_backup[i]->SetLineWidth(fH1_linewidth  );
        }

    To_display_histos(0);
}


void ROOTSCOPE::To_change_fillstyle() {

    Color_t fillColor;
    fH1_fillstyle ++ ;
    if( fH1_fillstyle %2 == 0 ) { fH1_fillstyle =0; }



    Int_t histoN = histos.size();
    if( fH1_fillstyle == 1 ) {

        for(Int_t i=0; i<histoN; i++) {
            histos[i]->SetFillStyle( 3004 );
            fillColor = histos[i]->GetLineColor();
            histos[i]->SetFillColor( fillColor );
            histos_backup[i]->SetFillStyle( 3004 );
            histos_backup[i]->SetFillColor( fillColor );
            }
        }
    else if ( fH1_fillstyle == 0 ) {
        for(Int_t i=0; i<histoN; i++) {
            histos[i]->SetFillStyle( 0 );
            histos_backup[i]->SetFillStyle( 0 );
            }
        }

    To_display_histos(0);

}



void ROOTSCOPE::To_change_style_2d( ){

    /* to cicle the style */

    int optionN = sizeof( styleDraw )/ sizeof( styleDraw[0] );
    fH2_style ++ ;
    if( fH2_style %optionN == 0 ) { fH2_style =0; }
    histo2d->Draw( styleDraw[fH2_style]  );
    c1->Update();
}


void ROOTSCOPE::To_change_marker_size(){

    /* to circle marker size */

    int optionN = sizeof( styleMarkersize )/ sizeof( styleMarkersize[0] );

    fH2_marker_style ++ ;
    if( fH2_marker_style %optionN == 0 ) { fH2_marker_style =0; }

    histo2d->SetMarkerStyle( styleMarkersize[fH2_marker_style] );
    histo2d_backup->SetMarkerStyle( styleMarkersize[fH2_marker_style] );
    fH2_style = 0;
    histo2d->Draw();
    c1->Update();
}


void ROOTSCOPE::Single_pad_setting() {

    fIsTH2_inPad = false;
    fIsSameDim = false; // single pad
    fPadActive = 0;     // single pad
    fPadTotalN = 1;     // single pad ( needed for Rebin_compress() )

}


void ROOTSCOPE::Set_CannotMove() {

    /* To avoid the mouse action move the Frame */

    c1 -> FindObject("TFrame")->SetBit(TBox::kCannotMove);



    // if we have multiple pads, set each of their frames not able to move.
    Int_t padN = 1; // start from 1, cd(0) set current(mother) pad
    while( c1->GetPad( padN) ) { padN++;  }

    for( Int_t i= 0; i< padN; i++ ){

        c1->GetPad( i )->FindObject("TFrame")->SetBit(TBox::kCannotMove);

    }

}


TH2* ROOTSCOPE::GetTwoDHisto(){

    return histo2d;
}


TH1* ROOTSCOPE::GetOneDHisto(){

    return Get_histo_by_padnum( fPadActive );

}



bool ROOTSCOPE::Remove_default_empty_histo() {

    bool flag = false;

    // to remove the default empty hisogram
    if( histos.size() == 1 )
    {

        TString name = histos[0]->GetName();

        if( name == "hempty123xyz" )
        {

            histos.erase( histos.begin() );
            flag = true;
            return flag;
        }
    }
    return flag;
}



void ROOTSCOPE::AddOneDHisto( TH1* h_input ) {

    Bool_t IsChange = Remove_default_empty_histo();

    histos.push_back( h_input );
    To_backup_histos();
    *fText_viewer <<"add a 1d histogram" << endl;
    fText_viewer->ShowBottom();

    if( IsChange ) {
        histo = histos[0];
        fOpen_list.clear();
        fOpen_list.push_back( 0 ); // show the one we just add.
        Single_pad_setting(); // single pad
        To_display_histos( 1 );

    }



}


void ROOTSCOPE::SetTwoDHisto( TH2* h2d_input) {

    Bool_t IsChange = Remove_default_empty_histo();

    histo2d = h2d_input;
    histo2d_backup = (TH2*) h2d_input->Clone();
    fHasTH2 = true;
    *fText_viewer <<"Load a 2d histogram" << endl;
    fText_viewer->ShowBottom();

    if( IsChange ) {
        histos.insert( histos.begin(), histo2d->ProjectionY("Y_PROJ_ALL")  );
        histos.insert( histos.begin(), histo2d->ProjectionX("X_PROJ_ALL")  );
        histos.at(0)->SetTitle("FullYgate_ProjX");
        histos.at(1)->SetTitle("FullXgate_ProjY");
        fOpen_list.clear();
        fOpen_list.push_back( 0 );
        fOpen_list.push_back( 1 );
        To_display_histos( 1 );

    }

}




// constructor for receiving a 1D histogram.

ROOTSCOPE::ROOTSCOPE( const TGWindow * p,  TH1* histo_input )
: TGMainFrame(p, 500, 400)
{
    SetCleanup(kDeepCleanup); // important step for closing windows properly.

    Initialization();

    histo = histo_input;
    histo_backup = (TH1*) histo_input->Clone(); // backup, will be used when rebin.



    Create_Widgets( 500, 400 );


    fHasTH2 = false;     // we don't have a histo2d yet.
    fIsSameDim = false;  // false for one pad initially
    fPadTotalN = 1;


     //---- Initial Setting -------//
    histos.push_back( histo  );
    To_backup_histos();
    fOpen_list.push_back( 0 );
    To_display_histos( 1 );


}



// constructor for receiving a 2D histogram.

ROOTSCOPE::ROOTSCOPE( const TGWindow * p, TH2* histo2d_input )
: TGMainFrame(p, 500, 400)
{
    SetCleanup(kDeepCleanup); // important step for closing windows properly.

    Initialization();

    Create_Widgets( 500, 400 );

    histo2d = histo2d_input;
    histo2d_backup = (TH2*) histo2d_input->Clone(); // backup, will be used when rebin



    //---- Initial Setting -------//
    // to create two spectra from projection all x and y axis.
    histos.push_back( histo2d->ProjectionX("X_PROJ_ALL") );
    histos.push_back( histo2d->ProjectionY("Y_PROJ_ALL") );

    histos.at(0)->SetTitle("FullYgate_ProjX");
    histos.at(1)->SetTitle("FullXgate_ProjY");
    To_backup_histos();

    fOpen_list.push_back( 0 );
    fOpen_list.push_back( 1 );
    To_display_histos( 1 );

}


// constructor for receiving nothing.

ROOTSCOPE::ROOTSCOPE( const TGWindow * p  )
: TGMainFrame( p, 500, 400 )
{
    SetCleanup(kDeepCleanup); // important step for closing windows properly.

    Initialization();

    Create_Widgets( 500, 400 );



    //  Now we have no pad, and To_respons() will complain.
    //  So we have to make a temp histo.

    TH1* hempty = new TH1F( "hempty123xyz", "empty", 100,-1,1 );
    histo = hempty;
    histo_backup = (TH1*) hempty->Clone();


    fHasTH2 = false;     // we don't have a histo2d yet.
    fIsSameDim = false;  // false for one pad initially
    fPadTotalN = 1;


     //---- Initial Setting -------//
    histos.push_back( histo  );
    To_backup_histos();
    fOpen_list.push_back( 0 );
    To_display_histos( 0 );

    *fText_viewer
    << "to load 1d/2d histogram:\n"
    << "using AddOneDHisto( TH1* h_input )\n"
    << "using SetTwoDHisto( TH2* h2d_input )\n"
    << "or by ctrl+r to load the root file.\n"
    <<  endl;

    fText_viewer->ShowBottom();

}

// constructor for receiving a root file

ROOTSCOPE::ROOTSCOPE( const TGWindow * p, const char* rootFileName  )
: TGMainFrame( p, 500, 400 )
{
    SetCleanup(kDeepCleanup); // important step for closing windows properly.

    Initialization();



    TString inFileName = rootFileName;
    Bool_t  isOK  = false;

    // here we test if we can read the root file.
    TFile fTesting(inFileName.Data());
    if( fTesting.IsOpen() ) {
        isOK = true;
        printf( "open \"%s\" \n", inFileName.Data() );
    } else { printf( "CANNOT FIND %s\n", inFileName.Data()); }
    fTesting.Close();


    if( isOK ) {

        Create_Widgets( 500, 400 );

        ReadIn_and_parse* tmpManager
        = new ReadIn_and_parse( &histos, inFileName  ); // update histos here.



        fHasTH2 = tmpManager->Get_hasTH2();

        if( fHasTH2 ) {
            // have TH2
            histo2d = tmpManager->Get_TH2();

            // insert the total projections to histos
            histos.insert( histos.begin(),
                histo2d->ProjectionY( Form( "%s%f", "Y_PROJ_ALL", gRandom->Uniform() ) ) );
            histos.insert( histos.begin(),
                histo2d->ProjectionX( Form( "%s%f", "X_PROJ_ALL", gRandom->Uniform() ) ) );
            histos.at(0)->SetTitle("FullYgate_ProjX_atuo");
            histos.at(1)->SetTitle("FullXgate_ProjY_auto");
            histo2d_backup = (TH2*) histo2d->Clone();
            fOpen_list.push_back( 0 );
            fOpen_list.push_back( 1 );
            To_display_histos( 1 );
            }
        else if ( histos.size() > 0 && !fHasTH2 ) {
            // no TH2, but we have TH1s
            // to display the first hitogram.
            histo = histos[0];
            fOpen_list.clear();
            fOpen_list.push_back( 0 );
            Single_pad_setting(); // single pad
            To_display_histos( 1 );
        }
        else{

            //  no 1d/2d hitogram in the root file...
            //  Now we have no pad, and To_response_key() will complain.
            //  So we have to make a temp histo.

            TH1* hempty = new TH1F( "hempty123xyz", "empty", 100,-1,1 );
            histo = hempty;
            histo_backup = (TH1*) hempty->Clone();
            fHasTH2 = false;     // we don't have a histo2d yet.
            fIsSameDim = false;  // false for one pad initially
            fPadTotalN = 1;

            //---- Initial Setting -------//
            histos.push_back( histo  );
            fOpen_list.push_back( 0 );
            To_display_histos( 0 );

            *fText_viewer
            << "to load 1d/2d histogram:\n"
            << "using AddOneDHisto( TH1* h_input )\n"
            << "using SetTwoDHisto( TH2* h2d_input )\n"
            << "or by ctrl+r to load the root file.\n"
            <<  endl;

            fText_viewer->ShowBottom();

        }

        To_backup_histos();


    }


}




void myROOTSCOPE()
{
    printf( "~Test~\n");

    // prepare some histograms
    TH2F* htwoD = new TH2F("htwoD","htwoD",100,-3,3,100,-3, 3 );

    for( int i=0; i< 10000; i++) {
      htwoD->Fill( gRandom->Gaus(0,0.2), gRandom->Gaus(0, 0.8) );
    }


    TH1F* honeD = new TH1F( "honeD", "honeD", 100,-3,3 );
    honeD->FillRandom( "gaus", 1000 );



    // demo: constructor usage
    // load 2d histo
    // we will automatically get full projections on X and Y.
    if( 0 ) {

        ROOTSCOPE* app1
        = new ROOTSCOPE ( gClient->GetRoot(),  htwoD );
    }


    // load a 1d histo
    // we don't have a 2d histo, and we can add 2d histo later.
    if ( 0 ) {
        ROOTSCOPE* app2
        = new ROOTSCOPE ( gClient->GetRoot(),  honeD );
    }


    // initially we don't load anything
    // and then we add 1d/2d histograms into it.
    if( 1 ) {

        ROOTSCOPE* app3
        = new ROOTSCOPE( gClient->GetRoot() );

        app3->SetTwoDHisto( htwoD );
        app3->AddOneDHisto( htwoD->ProjectionX() );
        app3->AddOneDHisto( htwoD->ProjectionY() );

        app3->AddOneDHisto( honeD );
    }

    // initially 1d/2d histogram(s) from a rootfile.
    if( 0 ) {

        ROOTSCOPE* app4
        //= new ROOTSCOPE( gClient->GetRoot(), "just_for_test.root" );
        = new ROOTSCOPE( gClient->GetRoot(), "/home/xination/Dropbox/3_my_program/demo_root/demo_histo/A_demo_sqr.root" );
    }



}

