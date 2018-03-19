//
//   Author : Pei-Luan Tai
//   Contact: pt10f@my.fsu.edu
//   Last update: Dec 3, 2017
//***************************************/
#include <vector>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TFile.h>
#include <TObjArray.h>
#include <TString.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGTextEntry.h>
#include <TGFileDialog.h>
#include <TGNumberEntry.h>
#include <TGWindow.h>
#include <TKey.h>
#include <TVirtualX.h>
#include <KeySymbols.h>
#include <TObjString.h>
#include <TG3DLine.h>
#include <TRandom.h>


using namespace std;
////////////////////////////////////////////////////////////////////////////////////////////////////



class Dlg_Set_Background: public TGTransientFrame {

    float*  fbg;

    TGHorizontalFrame*  fHF[2];
    TGLabel*            fLabel[2];
    TGNumberEntry*      fNumericEntries[2];

    TGTextButton*       fBtn_close;
    static const char *const label_text[2]; /* the text on the label */


public:

    // class constructor
    Dlg_Set_Background( const TGWindow* ,    /* base windows */
                        const TGWindow*,     /* the windows transient from */
                        UInt_t ,            /* width */
                        UInt_t ,            /* height */
                        float* );           /* bg value */

    void    To_response_key( Event_t* );

    void    To_focus_on_entry( Event_t* );

    void    CloseWindow();

    void    Update_bg_pol0(char*  );


};



const char *const Dlg_Set_Background::label_text[2] = {
   "background = ",
   "just for test"
};




void Dlg_Set_Background::CloseWindow(){

    fBtn_close->SetState(kButtonDisabled); // to avoid double click.

    DeleteWindow();
 }




void Dlg_Set_Background::To_focus_on_entry( Event_t* e) {

    fNumericEntries[0]->GetNumberEntry()->SetFocus();
    fNumericEntries[0]->GetNumberEntry()->SelectAll();
}


void Dlg_Set_Background::To_response_key(Event_t* e) {

    if ( e->fType == kGKeyPress   )
    {
        // using LookuString to get the key mapping.
        UInt_t key_symbol;
        char tmp[2];
        gVirtualX->LookupString( e, tmp,sizeof(tmp), key_symbol );

        const unsigned key_enter = 36;

        // the enter key near num pad
        if( key_symbol == kKey_Enter   ) {  Dlg_Set_Background::CloseWindow(); }

        // the enter key in regular position.
        else if ( e->fCode == key_enter ) {  Dlg_Set_Background::CloseWindow(); }

    }

}


// class constructor
Dlg_Set_Background::Dlg_Set_Background( const TGWindow* p,
                                        const TGWindow* main,
                                        UInt_t w,
                                        UInt_t h,
                                        float* bg )
: TGTransientFrame(p, main, w, h)
{

    fbg = bg;

    *fbg = 0; // default value ( if user never change the text entry )

    SetCleanup(kDeepCleanup); // important step for closing windows properly.

    TGLayoutHints*  Layout1 = new TGLayoutHints( kLHintsCenterY, 2, 2, 2, 2);


    //-------------------------------------------------------------------| Label-Entry pair
    for (int i = 0; i < 1; i++)
    {
        fHF[i] = new TGHorizontalFrame( this , 200, 30);

        AddFrame(fHF[i], Layout1 );

        fLabel[i] = new TGLabel( fHF[i], label_text[i]);  /* base, text */

        fNumericEntries[i] =
        new TGNumberEntry(  fHF[i],     /* base */
                            0,          /* init value */
                            12,         /* digit width */
                            i,          /* ID */
                            TGNumberFormat::kNESRealOne);

        fHF[i]->AddFrame( fLabel[i], Layout1 );
        fHF[i]->AddFrame( fNumericEntries[i], Layout1 );

    }



    //------------------------------------------------------------------| Connect
    fNumericEntries[0]->GetNumberEntry()
        -> Connect( "TextChanged(char*)",
                    "Dlg_Set_Background", this, "Update_bg_pol0(char*)");

    fNumericEntries[0]->GetNumberEntry()
        -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_Set_Background", this,
                    "To_response_key(Event_t*)");



    //------------------------------------------------------| A Button for close the window
    TGLayoutHints*  Layout2 = new TGLayoutHints( kLHintsExpandX, 2, 2, 2, 2);
    fBtn_close = new TGTextButton( this, "OK" );
    fBtn_close -> Connect( "Clicked()", "Dlg_Set_Background", this, "CloseWindow()"  );
    AddFrame( fBtn_close, Layout2);


    //-----------------------------------------------------------to focus on entry
    this->Connect(  "ProcessedEvent(Event_t*)", "Dlg_Set_Background", this,
                    "To_focus_on_entry(Event_t*)" );


    SetName("Dlg_Set_Background");
    SetWindowName("Set Background");
    MapSubwindows();
    Resize( GetDefaultSize() );
    MapWindow();

    gClient->WaitFor(this);

}


// update the customized bg value
void Dlg_Set_Background::Update_bg_pol0(char* entry_output){

    TString s = entry_output;

    float bg_value = (float) s.Atof();

    *fbg = bg_value;

}


/////////////////////////////////////////////////////////////////////////////////////



class Dlg_Set_Marker: public TGTransientFrame {

    float*  fXpos;

    TGHorizontalFrame*  fHF[2];
    TGLabel*            fLabel[2];
    TGNumberEntry*      fNumericEntries[2];

    TGTextButton*       fBtn_close;



public:

    // class constructor
    Dlg_Set_Marker( const TGWindow* ,    /* base windows */
                    const TGWindow*,     /* the windows transient from */
                    UInt_t ,            /* width */
                    UInt_t ,            /* height */
                    float* ,            /* bg value passing by reference */
                    float  ,
                    float );
    void    To_response_key( Event_t* );

    void    To_focus_on_entry( Event_t* );

    void    CloseWindow();

    void    Update_marker(char*  );


};






void Dlg_Set_Marker::To_focus_on_entry( Event_t* e) {

    fNumericEntries[0]->GetNumberEntry()->SetFocus();
    fNumericEntries[0]->GetNumberEntry()->SelectAll();
}


void Dlg_Set_Marker::To_response_key(Event_t* e) {

    if ( e->fType == kGKeyPress   )
    {
        // using LookuString to get the key mapping.
        UInt_t key_symbol;
        char tmp[2];
        gVirtualX->LookupString( e, tmp,sizeof(tmp), key_symbol );

         const unsigned key_enter = 36;

        // the enter key near num pad
        if( key_symbol == kKey_Enter   ) {  Dlg_Set_Marker::CloseWindow(); }

        // the enter key in regular position.
        else if ( e->fCode == key_enter ) {  Dlg_Set_Marker::CloseWindow(); }

    }

}



// update the customized bg value
void Dlg_Set_Marker::Update_marker(char* entry_output){

        TString s = entry_output;
        float marker_value = (float) s.Atof();
        *fXpos = marker_value;
}




void Dlg_Set_Marker::CloseWindow(){

    fBtn_close->SetState(kButtonDisabled); // to avoid double click.
    DeleteWindow();
 }



// class constructor
Dlg_Set_Marker::Dlg_Set_Marker( const TGWindow* p,
                                const TGWindow* main,
                                UInt_t w,
                                UInt_t h,
                                float* xpos,
                                float  limit_lower, float limit_upper )
: TGTransientFrame(p, main, w, h)
{

    fXpos = xpos;
    *fXpos = 0;  // default value, if user doesn't change the text entry.


    // current just one label text.
    TString label_text = Form( "form %.2f to %.2f", limit_lower, limit_upper );


    SetCleanup(kDeepCleanup); // important step for closing windows properly.

    TGLayoutHints*  Layout1 = new TGLayoutHints( kLHintsCenterY, 2, 2, 2, 2);


    //-------------------------------------------------------------------| Label-Entry pair
    for (int i = 0; i < 1; i++)
    {
        fHF[i] = new TGHorizontalFrame( this , 200, 30);

        AddFrame(fHF[i], Layout1 );

        fLabel[i] = new TGLabel( fHF[i], label_text.Data() );  /* base, text */

        fNumericEntries[i] =
        new TGNumberEntry(  fHF[i],     /* base */
                            0,          /* init value */
                            12,         /* digit width */
                            i,          /* ID */
                            TGNumberFormat::kNESRealOne);


        fHF[i]->AddFrame( fNumericEntries[i], Layout1 );
        fHF[i]->AddFrame( fLabel[i], Layout1 );


    }


    //------------------------------------------------------------------| Connect
    fNumericEntries[0]->GetNumberEntry()
        -> Connect( "TextChanged(char*)",
                    "Dlg_Set_Marker", this, "Update_marker(char*)");

    fNumericEntries[0]->GetNumberEntry()
        -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_Set_Marker", this,
                    "To_response_key(Event_t*)");



    //-------------------------------------------------------------------| A Button for close the window
    TGLayoutHints*  Layout2 = new TGLayoutHints( kLHintsExpandX, 2, 2, 2, 2);
    fBtn_close = new TGTextButton( this, "OK" );
    fBtn_close -> Connect( "Clicked()", "Dlg_Set_Marker", this, "CloseWindow()"  );
    AddFrame( fBtn_close, Layout2);



    //-----------------------------------------------------------to focus on entry
    this->Connect(  "ProcessedEvent(Event_t*)", "Dlg_Set_Marker", this,
                    "To_focus_on_entry(Event_t*)" );



    SetName("dlg_set_marker");
    SetWindowName("Set Marker");
    MapSubwindows();
    Resize( GetDefaultSize() );
    MapWindow();


    gClient->WaitFor(this);


}






/////////////////////////////////////////////////////////////////////////////////////


class Dlg_Set_YRange: public TGTransientFrame {

    float*  fYRange;
    float   fYRange_backup;

    TGHorizontalFrame*  fHF[2];
    TGHorizontalFrame*  fHF_btns;
    TGLabel*            fLabel[2];
    TGNumberEntry*      fNumericEntries[2];

    TGTextButton*       fBtn_close;
    TGTextButton*       fBtn_cancel;



public:

    // class constructor
    Dlg_Set_YRange( const TGWindow* ,    /* base windows */
                    const TGWindow*,     /* the windows transient from */
                    UInt_t ,            /* width */
                    UInt_t ,            /* height */
                    float*,             /* value passing by reference */
                    float );            /* original value */

    void    To_response_key( Event_t* );

    void    To_focus_on_entry( Event_t* );

    void    Cancel();

    void    CloseWindow();

    void    Update_marker(char*  );


};



// update the customized bg value
void Dlg_Set_YRange::Update_marker(char* entry_output){

        float marker_value = (float) atoi(entry_output);

        *fYRange = marker_value;

}



void Dlg_Set_YRange::Cancel(){

    *fYRange = fYRange_backup;
    CloseWindow();


 }


void Dlg_Set_YRange::CloseWindow(){


    fBtn_close->SetState(kButtonDisabled); // to avoid double click.
    fBtn_cancel->SetState(kButtonDisabled);
    DeleteWindow();


 }


void Dlg_Set_YRange::To_focus_on_entry( Event_t* e) {

    fNumericEntries[0]->GetNumberEntry()->SetFocus();
    fNumericEntries[0]->GetNumberEntry()->SelectAll();
}



void Dlg_Set_YRange::To_response_key(Event_t* e) {

    if ( e->fType == kGKeyPress   )
    {
        // using LookuString to get the key mapping.
        UInt_t key_symbol;
        char tmp[2];
        gVirtualX->LookupString( e, tmp,sizeof(tmp), key_symbol );

         const unsigned key_enter = 36;

        if( key_symbol == kKey_Enter   ) {  CloseWindow(); } // the enter key near num pad

        else if ( e->fCode == key_enter ) {  CloseWindow(); } // the enter key in regular position.

        else if( key_symbol == kKey_Escape ) {  CloseWindow(); }

    }

}




// class constructor
Dlg_Set_YRange::Dlg_Set_YRange( const TGWindow* p,
                                const TGWindow* main,
                                UInt_t w,
                                UInt_t h,
                                float* yrange,
                                float  yrange_original )
: TGTransientFrame(p, main, w, h)
{

    fYRange = yrange;
    fYRange_backup = yrange_original;
    *fYRange = yrange_original;  // default value, if user doesn't change the text entry.

    // current I just set one label text.
    TString label_text = Form( " customized y max " );


    SetCleanup(kDeepCleanup); // important step for closing windows properly.

    TGLayoutHints*  Layout1 = new TGLayoutHints( kLHintsCenterY, 2, 2, 2, 2);


    //-------------------------------------------------------------------| Label-Entry pair
    for (int i = 0; i < 1; i++)
    {
        fHF[i] = new TGHorizontalFrame( this , 200, 30);

        AddFrame(fHF[i], Layout1 );

        fLabel[i] = new TGLabel( fHF[i], label_text.Data() );  /* base, text */

        fNumericEntries[i] =
        new TGNumberEntry(  fHF[i],     /* base */
                            *fYRange,   /* init value */
                            12,         /* digit width */
                            i,          /* ID */
                            TGNumberFormat::kNESInteger);


        fHF[i]->AddFrame( fNumericEntries[i], Layout1 );
        fHF[i]->AddFrame( fLabel[i], Layout1 );


    }



    //------------------------------------------------------------------| Connect
    fNumericEntries[0]->GetNumberEntry()
        -> Connect( "TextChanged(char*)",
                    "Dlg_Set_YRange", this, "Update_marker(char*)");

    fNumericEntries[0]->GetNumberEntry()
        -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_Set_YRange", this,
                    "To_response_key(Event_t*)");


    TGLayoutHints*  Layout2 = new TGLayoutHints( kLHintsExpandX, 2, 2, 2, 2);
    fHF_btns = new TGHorizontalFrame( this , 200, 30);

    //-------------------------------------------------------------------| Btn for close the window
    fBtn_close = new TGTextButton( fHF_btns, "OK" );
    fBtn_close -> Connect( "Clicked()", "Dlg_Set_YRange", this, "CloseWindow()"  );
    fHF_btns->AddFrame( fBtn_close, Layout2);

    //-------------------------------------------------------------------| Btn for cancel the change

    fBtn_cancel = new TGTextButton( fHF_btns, "Cancel" );
    fBtn_cancel -> Connect( "Clicked()", "Dlg_Set_YRange", this, "Cancel()"  );
    fHF_btns->AddFrame( fBtn_cancel, Layout2);


    AddFrame( fHF_btns, Layout2);



    //-----------------------------------------------------------to focus on entry
    this->Connect(  "ProcessedEvent(Event_t*)", "Dlg_Set_YRange", this,
                    "To_focus_on_entry(Event_t*)" );

    SetName("dlg_set_YRange");
    SetWindowName("Set Y Max");
    MapSubwindows();
    Resize( GetDefaultSize() );
    MapWindow();

    gClient->WaitFor(this);

}






/////////////////////////////////////////////////////////////////////////////////////



class Dlg_Set_hTitle: public TGTransientFrame {

    TString*  fhtitle;
    TString   fhtitle_backup;

    TGVerticalFrame*    fVF;
    TGHorizontalFrame*  fHF_1;
    TGHorizontalFrame*  fHF_2;



    TGLabel*            fLabel;
    TGTextEntry*        fTextEntries;
    TGTextButton*       fBtn_close;
    TGTextButton*       fBtn_cancel;



public:

    // class constructor
    Dlg_Set_hTitle( const TGWindow* ,    /* base windows */
                    const TGWindow*,     /* the windows transient from */
                    UInt_t ,             /* width */
                    UInt_t ,             /* height */
                    TString* );

    void    To_response_key( Event_t* );

    void    To_focus_on_entry( Event_t* );

    void    CloseWindow();

    void    Cancel();

    void    Update_title(char*  );


};




void Dlg_Set_hTitle::Update_title(char* entry_output){


        *fhtitle = entry_output;

}


void Dlg_Set_hTitle::Cancel(){

    // cancel the change, and use the original title.
    *fhtitle = fhtitle_backup.Data();
    CloseWindow();
 }


void Dlg_Set_hTitle::CloseWindow(){

    fBtn_close->SetState(kButtonDisabled); // to avoid double click.
    fBtn_cancel->SetState(kButtonDisabled);

    DeleteWindow();


 }



void Dlg_Set_hTitle::To_focus_on_entry( Event_t* e) {

    fTextEntries->SetFocus();
    fTextEntries->SelectAll();
}


void Dlg_Set_hTitle::To_response_key(Event_t* e) {

    if ( e->fType == kGKeyPress   )
    {
        // using LookuString to get the key mapping.
        UInt_t key_symbol;
        char tmp[2];
        gVirtualX->LookupString( e, tmp,sizeof(tmp), key_symbol );

         const unsigned key_enter = 36;

        if( key_symbol == kKey_Enter   )  {  CloseWindow(); } // the enter key near num pad

        else if ( e->fCode == key_enter ) {  CloseWindow(); } // the enter key in regular position.

        else if( key_symbol == kKey_Escape   )  {  CloseWindow(); }

    }

}




// class constructor
Dlg_Set_hTitle::Dlg_Set_hTitle( const TGWindow* p,
                                const TGWindow* main,
                                UInt_t w,
                                UInt_t h,
                                TString* title)
: TGTransientFrame(p, main, w, h)
{

    SetCleanup(kDeepCleanup); // important step for closing windows properly.

    fhtitle = title;
    fhtitle_backup= title->Copy();



    TGLayoutHints*  Layout1 = new TGLayoutHints( kLHintsCenterY, 2, 2, 2, 2);
    TGLayoutHints*  Layout2 = new TGLayoutHints( kLHintsExpandX, 2, 2, 2, 2);



    fHF_1 = new TGHorizontalFrame( this , 200, 30);
    fHF_2 = new TGHorizontalFrame( this , 200, 30);

    //-------------------------------------------------------------------| text entry
    fTextEntries =
    new TGTextEntry(  fHF_1, fhtitle->Data(), 1);    /* base, init value, ID */

    fTextEntries->Resize(300, fTextEntries->GetDefaultHeight() );

    fHF_1->AddFrame( fTextEntries, Layout1 );



    //------------------------------------------------------------------| Connect
    fTextEntries
        -> Connect( "TextChanged(char*)",
                    "Dlg_Set_hTitle", this, "Update_title(char*)");

    fTextEntries
        -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_Set_hTitle", this,
                    "To_response_key(Event_t*)");


    //-------------------------------------------------------------------| btn: close the window

    fBtn_close = new TGTextButton( fHF_2, "  OK  " );
    fBtn_close -> Connect( "Clicked()", "Dlg_Set_hTitle", this, "CloseWindow()"  );
    fHF_2->AddFrame( fBtn_close, Layout2 );
    fBtn_close->Resize( 150, fBtn_close->GetDefaultHeight() );


    //-------------------------------------------------------------------| btn:  cancel the change
    fBtn_cancel = new TGTextButton( fHF_2, "Cancel" );
    fBtn_cancel -> Connect( "Clicked()", "Dlg_Set_hTitle", this, "Cancel()"  );
    fHF_2->AddFrame( fBtn_cancel, Layout2 );
    fBtn_cancel->Resize(150, fBtn_cancel->GetDefaultHeight() );

    AddFrame( fHF_1, Layout1 );
    AddFrame( fHF_2, Layout2 );



    //-----------------------------------------------------------to focus on entry
    this->Connect(  "ProcessedEvent(Event_t*)", "Dlg_Set_hTitle", this,
                    "To_focus_on_entry(Event_t*)" );




    SetName("Dlg_Set_hTitle");
    SetWindowName("Set histo title");
    MapSubwindows();
    Resize( GetDefaultSize() );
    MapWindow();

    gClient->WaitFor(this);


}



/////////////////////////////////////////////////////////////////////////////////////



class Dlg_Display_histos: public TGMainFrame {

private:

    vector<TH1*>    fHistos;
    TString*        fHisto_list;
    TString         fHisto_list_backup;

    TGVerticalFrame*    fVF;
    TGHorizontalFrame*  fHF_1;
    TGHorizontalFrame*  fHF_2;
    TGVerticalFrame*    fHisto_frame;
    TGVerticalFrame*    fHisto_frame2;
    TGLabel*	        fHisto_nameLabel[200]; // set a upper limit = 200

    TGLabel*            fLabel;
    TGTextEntry*        fTextEntries;
    TGTextButton*       fBtn_close;
    TGTextButton*       fBtn_cancel;



public:

    // class constructor
    Dlg_Display_histos(
        const TGWindow* ,       /* base windows */
        UInt_t ,                /* width */
        UInt_t ,                /* height */
        TString* histo_list,
        vector<TH1*>  histos );

    void    To_response_key( Event_t* );

    void    To_focus_on_entry( Event_t* );

    void    CloseWindow();

    void    Cancel();

    void    Update_display_histos(char*  );


};




void Dlg_Display_histos::Cancel(){

    // cancel the change, and use the original setting.
    *fHisto_list = fHisto_list_backup.Data();
    CloseWindow();
 }


void Dlg_Display_histos::CloseWindow(){

    fBtn_close->SetState(kButtonDisabled); // to avoid double click.
    fBtn_cancel->SetState(kButtonDisabled);

    DeleteWindow();

 }



void Dlg_Display_histos::To_focus_on_entry( Event_t* e) {

    fTextEntries->SetFocus();
    fTextEntries->SelectAll();
}


void Dlg_Display_histos::To_response_key(Event_t* e) {

    if ( e->fType == kGKeyPress   )
    {
        // using LookuString to get the key mapping.
        UInt_t key_symbol;
        char tmp[2];
        gVirtualX->LookupString( e, tmp,sizeof(tmp), key_symbol );

         const unsigned key_enter = 36;

        if( key_symbol == kKey_Enter   )  {  CloseWindow(); } // the enter key near num pad

        else if ( e->fCode == key_enter ) {  CloseWindow(); } // the enter key in regular position.

        else if( key_symbol == kKey_Escape   )  { CloseWindow(); }

    }

}



void Dlg_Display_histos::Update_display_histos(char* entry_output){


        *fHisto_list = entry_output;


}




// class constructor
Dlg_Display_histos::Dlg_Display_histos(
                const TGWindow* p,
                UInt_t w,
                UInt_t h,
                TString* histo_list,
                vector<TH1*>  histos  )
: TGMainFrame(p, w, h) {

    SetCleanup(kDeepCleanup); // important step for closing windows properly.
    fHisto_list = histo_list;
    fHisto_list_backup= histo_list->Copy();

    fHistos = histos;

    TGLayoutHints*  Layout1 = new TGLayoutHints( kLHintsCenterY, 2, 2, 2, 2);
    TGLayoutHints*  Layout2 = new TGLayoutHints( kLHintsExpandX, 2, 2, 2, 2);
    TGLayoutHints*  Layout3 = new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2);

    Int_t nRow = 10;
    Int_t nCol = 0;
    fHisto_frame =new TGVerticalFrame( this, 10, 10);
    fHisto_frame->SetLayoutManager ( new TGMatrixLayout( fHisto_frame , nRow, nCol) );


    fHisto_frame2 =new TGVerticalFrame( this, 10, 10);
    fHisto_frame2->SetLayoutManager ( new TGMatrixLayout( fHisto_frame2, nRow, nCol) );

    //-------------------------------------------------------| Labels

    TString outstr;
    TString histo_title;

    // to set 10 entries for each column.
    // for 101 to 200 entry we will put them into the second layer.
    int n0 = histos.size() ;
    int n1 = histos.size() / 10;
    int n2 = ( n1 + 1) *10;
    if (n2 <= 10 ) n2 =20;


    if( n0 <= 100 ) {

        for (int i=0; i<n2 ; i++)
        {

            if( i  <histos.size() ) {
                histo_title = fHistos.at(i)->GetTitle();

                // to avoid lenghty title, we only print the last 15 char
                if( histo_title.Length()>20 ) {
                    histo_title
                    = "..." + histo_title( histo_title.Length()-15, 15 ); }

                outstr = Form( " %2d:  %s  ", i+1 , histo_title.Data() ) ;
            }
            else {
                histo_title = "";
                outstr = Form( "" ) ;
            }


            fHisto_nameLabel[i] = new TGLabel( fHisto_frame, outstr );
            fHisto_frame->AddFrame ( fHisto_nameLabel[i], Layout3 );

        }

        AddFrame( fHisto_frame, Layout1 );

    }
    else {

        for (int i=0; i<100 ; i++) {

            if( i  <histos.size() ) {
                histo_title = fHistos.at(i)->GetTitle();
                // to avoid lenghty title, we only print the last 12 char
                if( histo_title.Length()>15 ) {
                    histo_title
                    = "..." + histo_title( histo_title.Length()-12, 12 ); }
                outstr = Form( " %2d:  %s  ", i+1 , histo_title.Data() ) ;
            }
            else {
                histo_title = "";
                outstr = Form( "" ) ;
            }


            fHisto_nameLabel[i] = new TGLabel( fHisto_frame, outstr );
            fHisto_frame->AddFrame ( fHisto_nameLabel[i], Layout3 );

        }
        AddFrame( fHisto_frame, Layout1 );

        Int_t n3 = TMath::Min( 200, n0 );

        for (int i=100; i<n0 ; i++)
        {

            if( i  <histos.size() ) {
                histo_title = fHistos.at(i)->GetTitle();
                // to avoid lenghty title, we only print the last 12 char
                if( histo_title.Length()>15 ) {
                    histo_title
                    = "..." + histo_title( histo_title.Length()-12, 12 ); }
                outstr = Form( " %3d:  %s  ", i+1 , histo_title.Data() ) ;
            }
            else {
                histo_title = "";
                outstr = Form( "" ) ;
            }


            fHisto_nameLabel[i] = new TGLabel( fHisto_frame2, outstr );
            fHisto_frame2->AddFrame ( fHisto_nameLabel[i], Layout3 );

        }

        AddFrame( fHisto_frame2, Layout1 );
    }

    //-------------------------------------------------------|






    fHF_1 = new TGHorizontalFrame( this , 1, 1);
    fHF_2 = new TGHorizontalFrame( this , 200, 30);


    //-------------------------------------------------------------------| text entry
    fTextEntries =
    new TGTextEntry(  fHF_1, fHisto_list->Data(), 1);    /* base, init value, ID */


    int width = fHisto_frame->GetDefaultWidth() ;
    fTextEntries->Resize( width, fTextEntries->GetDefaultHeight() );
    fHF_1->AddFrame( fTextEntries, Layout2 );



    //------------------------------------------------------------------| Connect
    fTextEntries
        -> Connect( "TextChanged(char*)",
                    "Dlg_Display_histos", this, "Update_display_histos(char*)");

    fTextEntries
        -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_Display_histos", this,
                    "To_response_key(Event_t*)");


    //-------------------------------------------------------------------| btn: close the window

    fBtn_close = new TGTextButton( fHF_2, "  OK  " );
    fBtn_close -> Connect( "Clicked()", "Dlg_Display_histos", this, "CloseWindow()"  );
    fHF_2->AddFrame( fBtn_close, Layout2 );
    fBtn_close->Resize( 150, fBtn_close->GetDefaultHeight() );


    //-------------------------------------------------------------------| btn:  cancel the change
    fBtn_cancel = new TGTextButton( fHF_2, "Cancel" );
    fBtn_cancel -> Connect( "Clicked()", "Dlg_Display_histos", this, "Cancel()"  );
    fHF_2->AddFrame( fBtn_cancel, Layout2 );
    fBtn_cancel->Resize(150, fBtn_cancel->GetDefaultHeight() );

    AddFrame( fHF_1, Layout1 );
    AddFrame( fHF_2, Layout2 );



    //-----------------------------------------------------------to focus on entry
    this->Connect(  "ProcessedEvent(Event_t*)", "Dlg_Display_histos", this,
                    "To_focus_on_entry(Event_t*)" );




    SetName("Dlg_Display_histos");
    SetWindowName("Display Histograms");
    MapSubwindows();
    Resize( GetDefaultSize() );
    MapWindow();
    gClient->WaitFor(this);


}
/////////////////////////////////////////////////////////////////////////////////////







class Dlg_Set_ZRange: public TGMainFrame {

    float*  fZRange;
    float   fZRange_backup;

    TString             fEntry_out;
    TGHorizontalFrame*  fHF[2];
    TGHorizontalFrame*  fHF_btns;
    TGLabel*            fLabel[2];
    TGNumberEntry*      fNumericEntries[2];

    TGTextButton*       fBtn_close;
    TGTextButton*       fBtn_cancel;



public:

    // class constructor
    Dlg_Set_ZRange( const TGWindow* ,    /* base windows */
                    UInt_t ,            /* width */
                    UInt_t ,            /* height */
                    float*,             /* value passing by reference */
                    float,             /* original value */
                    bool );

    void    To_response_key( Event_t* );

    void    To_focus_on_entry( Event_t* );

    void    Cancel();

    void    CloseWindow();

    void    Update_marker();

    void    Update_entry_output( char* entry_output );


};


void Dlg_Set_ZRange::Update_entry_output( char* entry_output ) {

    fEntry_out = entry_output;
}


// update the customized bg value
void Dlg_Set_ZRange::Update_marker( ){

        float marker_value = fEntry_out.Atoi();

        *fZRange = marker_value;

        CloseWindow();
}



void Dlg_Set_ZRange::Cancel(){

    *fZRange = fZRange_backup;
    CloseWindow();


 }


void Dlg_Set_ZRange::CloseWindow(){


    fBtn_close->SetState(kButtonDisabled); // to avoid double click.
    fBtn_cancel->SetState(kButtonDisabled);
    DeleteWindow();


 }


void Dlg_Set_ZRange::To_focus_on_entry( Event_t* e) {

    fNumericEntries[0]->GetNumberEntry()->SetFocus();
    fNumericEntries[0]->GetNumberEntry()->SelectAll();
}



void Dlg_Set_ZRange::To_response_key(Event_t* e) {

    if ( e->fType == kGKeyPress   )
    {
        // using LookuString to get the key mapping.
        UInt_t key_symbol;
        char tmp[2];
        gVirtualX->LookupString( e, tmp,sizeof(tmp), key_symbol );

         const unsigned key_enter = 36;

        if( key_symbol == kKey_Enter   ) {  Update_marker(); } // the enter key near num pad

        else if ( e->fCode == key_enter ) {  Update_marker(); } // the enter key in regular position.

        else if( key_symbol == kKey_Escape ) {  CloseWindow(); }

    }

}




// class constructor
Dlg_Set_ZRange::Dlg_Set_ZRange( const TGWindow* p,
                                UInt_t w,
                                UInt_t h,
                                float* zrange,
                                float  zrange_original,
                                bool toSetMax )
: TGMainFrame(p, w, h)
{

    fZRange = zrange;
    fZRange_backup = zrange_original;
    *fZRange = zrange_original;  // default value, if user doesn't change the text entry.



    TString label_text;
    if( toSetMax ) { label_text = Form( " customized z max " ); }
    else           { label_text = Form( " customized z min " ); }


    SetCleanup(kDeepCleanup); // important step for closing windows properly.

    TGLayoutHints*  Layout1 = new TGLayoutHints( kLHintsCenterY, 2, 2, 2, 2);


    //-------------------------------------------------------------------| Label-Entry pair
    for (int i = 0; i < 1; i++)
    {
        fHF[i] = new TGHorizontalFrame( this , 200, 30);

        AddFrame(fHF[i], Layout1 );

        fLabel[i] = new TGLabel( fHF[i], label_text.Data() );  /* base, text */

        fNumericEntries[i] =
        new TGNumberEntry(  fHF[i],     /* base */
                            *fZRange,   /* init value */
                            12,         /* digit width */
                            i,          /* ID */
                            TGNumberFormat::kNESInteger);


        fHF[i]->AddFrame( fNumericEntries[i], Layout1 );
        fHF[i]->AddFrame( fLabel[i], Layout1 );


    }



    //------------------------------------------------------------------| Connect
    fNumericEntries[0]->GetNumberEntry()
        -> Connect( "TextChanged(char*)",
                    "Dlg_Set_ZRange", this, "Update_entry_output(char*)");

    fNumericEntries[0]->GetNumberEntry()
        -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_Set_ZRange", this,
                    "To_response_key(Event_t*)");


    TGLayoutHints*  Layout2 = new TGLayoutHints( kLHintsExpandX, 2, 2, 2, 2);
    fHF_btns = new TGHorizontalFrame( this , 200, 30);

    //-------------------------------------------------------------------| Btn for close the window
    fBtn_close = new TGTextButton( fHF_btns, "OK" );
    fBtn_close -> Connect( "Clicked()", "Dlg_Set_ZRange", this, "CloseWindow()"  );
    fHF_btns->AddFrame( fBtn_close, Layout2);

    //-------------------------------------------------------------------| Btn for cancel the change

    fBtn_cancel = new TGTextButton( fHF_btns, "Cancel" );
    fBtn_cancel -> Connect( "Clicked()", "Dlg_Set_ZRange", this, "Cancel()"  );
    fHF_btns->AddFrame( fBtn_cancel, Layout2);


    AddFrame( fHF_btns, Layout2);



    //-----------------------------------------------------------to focus on entry
    this->Connect(  "ProcessedEvent(Event_t*)", "Dlg_Set_ZRange", this,
                    "To_focus_on_entry(Event_t*)" );


    if( toSetMax ) {
        SetName("Dlg_Set_ZRange_Max");
        SetWindowName("Set Z Max");
    }
    else {
        SetName("Dlg_Set_ZRange_Min");
        SetWindowName("Set Z Min");
    }

    MapSubwindows();
    Resize( GetDefaultSize() );
    MapWindow();

    gClient->WaitFor(this);

}

////////////////////////////////////////////////





class Dlg_Set_XRange: public TGMainFrame {

    float*  fUserMax;
    float*  fUserMin;


    TString             fEntry_out;
    TGHorizontalFrame*  fHF;
    TGHorizontalFrame*  fHF_btns;
    TGLabel*            fLabel;

    TGTextEntry*        fTextEntries;


    TGTextButton*       fBtn_close;
    TGTextButton*       fBtn_cancel;



public:

    // class constructor
    Dlg_Set_XRange( const TGWindow* p,
                UInt_t w,
                UInt_t h,
                float  primitive_xmin,
                float  primitive_xmax,
                float* user_min,
                float* user_max );

    void    To_response_key( Event_t* );

    void    To_focus_on_entry( Event_t* );

    void    Cancel();

    void    CloseWindow();

    void    Update_marker();

    void    Update_entry_output( char* entry_output );

    vector<const char*> ToParseString( TString  , const char*   );

};





void Dlg_Set_XRange::Update_entry_output( char* entry_output ) {

    fEntry_out = entry_output;
}


// my function to parse a TString which is split by delim
vector<const char*> Dlg_Set_XRange::ToParseString
( TString s_input , const char* delim  )
{

    TObjArray* objarray = s_input.Tokenize( delim );
    Int_t substringN = objarray->GetEntries();
    vector<const char*> substrings;



    for( Int_t i = 0; i < substringN; i++ )
    {
        TString tmp =  ( (TObjString*)objarray->At(i) ) -> GetString();

        substrings.push_back( Form( "%s", tmp.Data() )  );

    }

    return substrings;
}


// update the customized bg value
void Dlg_Set_XRange::Update_marker( ){

        if( fEntry_out != "" )
        {

            vector<const char*> ranges = ToParseString( fEntry_out, " " );
            if( ranges.size()>=2 ) {
                TString s1 = ranges[0] ;
                TString s2 = ranges[1] ;

                *fUserMax = TMath::Max(   s1.Atof(),   s2.Atof() );
                *fUserMin = TMath::Min(   s1.Atof(),   s2.Atof() );
                CloseWindow();

            } else {  /* not update fUserMax and fUserMin.*/  }


        }

}



void Dlg_Set_XRange::Cancel(){

    fUserMax = 0;
    fUserMin = 0;
    CloseWindow();


 }


void Dlg_Set_XRange::CloseWindow(){


    fBtn_close->SetState(kButtonDisabled); // to avoid double click.
    fBtn_cancel->SetState(kButtonDisabled);
    DeleteWindow();


 }


void Dlg_Set_XRange::To_focus_on_entry( Event_t* e) {

    fTextEntries->SetFocus();
    fTextEntries->SelectAll();
}



void Dlg_Set_XRange::To_response_key(Event_t* e) {

    if ( e->fType == kGKeyPress   )
    {
        // using LookuString to get the key mapping.
        UInt_t key_symbol;
        char tmp[2];
        gVirtualX->LookupString( e, tmp,sizeof(tmp), key_symbol );

         const unsigned key_enter = 36;

        if( key_symbol == kKey_Enter   ) {  Update_marker(); } // the enter key near num pad

        else if ( e->fCode == key_enter ) {  Update_marker(); } // the enter key in regular position.

        else if( key_symbol == kKey_Escape ) {  CloseWindow(); }

    }

}




// class constructor
Dlg_Set_XRange::Dlg_Set_XRange( const TGWindow* p,
                                UInt_t w,
                                UInt_t h,
                                float  primitive_xmin,
                                float  primitive_xmax,
                                float* user_min,
                                float* user_max )
: TGMainFrame(p, w, h)
{

    fUserMax = user_max;
    fUserMin = user_min;


    // for user know the range
    TString label_text = Form( " [%.1f to %.1f] ", primitive_xmin, primitive_xmax );



    SetCleanup(kDeepCleanup); // important step for closing windows properly.

    TGLayoutHints*  Layout1 = new TGLayoutHints( kLHintsCenterY, 2, 2, 2, 2);


    //-------------------------------------------------------------------| Label-Entry pair

    fHF = new TGHorizontalFrame( this , 200, 30);

    AddFrame( fHF, Layout1 );


    fLabel = new TGLabel( fHF, label_text.Data() );  /* base, text */

    fTextEntries = new TGTextEntry(  fHF, "", 1);   /* base, inital txt, ID */




    fHF->AddFrame( fTextEntries, Layout1 );
    fHF->AddFrame( fLabel, Layout1 );





    //------------------------------------------------------------------| Connect
    fTextEntries
        -> Connect( "TextChanged(char*)",
                    "Dlg_Set_XRange", this, "Update_entry_output(char*)");

    fTextEntries
        -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_Set_XRange", this,
                    "To_response_key(Event_t*)");


    TGLayoutHints*  Layout2 = new TGLayoutHints( kLHintsExpandX, 2, 2, 2, 2);
    fHF_btns = new TGHorizontalFrame( this , 200, 30);

    //-------------------------------------------------------------------| Btn for OK
    fBtn_close = new TGTextButton( fHF_btns, "OK" );
    fBtn_close -> Connect( "Clicked()", "Dlg_Set_XRange", this, "Update_marker()"  );
    fHF_btns->AddFrame( fBtn_close, Layout2);

    //-------------------------------------------------------------------| Btn for cancel the change

    fBtn_cancel = new TGTextButton( fHF_btns, "Cancel" );
    fBtn_cancel -> Connect( "Clicked()", "Dlg_Set_XRange", this, "Cancel()"  );
    fHF_btns->AddFrame( fBtn_cancel, Layout2);


    AddFrame( fHF_btns, Layout2);



    //-----------------------------------------------------------to focus on entry
    this->Connect(  "ProcessedEvent(Event_t*)", "Dlg_Set_XRange", this,
                    "To_focus_on_entry(Event_t*)" );



    SetName("Dlg_Set_X_Expand_Range");
    SetWindowName("Set X Expand Range");


    MapSubwindows();
    Resize( GetDefaultSize() );
    MapWindow();

    gClient->WaitFor(this);

}


///////////////////////////////////////////////////////////////////


class Dlg_Change_histo_X_Range: public TGMainFrame {

    float*  fXup_select;
    float*  fXlow_select;
    float   fXlow_backup;
    float   fXup_backup;


    TString             fEntry_out;
    TGHorizontalFrame*  fHF;
    TGHorizontalFrame*  fHF_btns;
    TGLabel*            fLabel;

    TGTextEntry*        fTextEntries;


    TGTextButton*       fBtn_close;
    TGTextButton*       fBtn_cancel;



public:

    // class constructor
    Dlg_Change_histo_X_Range( const TGWindow* p,
                UInt_t w,
                UInt_t h,
                float  primitive_xmin,
                float  primitive_xmax,
                float* user_min,
                float* user_max );

    void    To_response_key( Event_t* );

    void    To_focus_on_entry( Event_t* );

    void    Cancel();

    void    CloseWindow();

    void    Update_marker();

    void    Update_entry_output( char* entry_output );

    vector<const char*> ToParseString( TString  , const char*   );

};


void Dlg_Change_histo_X_Range::Update_entry_output( char* entry_output ) {

    fEntry_out = entry_output;
}

// my function to parse a TString which is split by delim
vector<const char*> Dlg_Change_histo_X_Range::ToParseString
( TString s_input , const char* delim  )
{

    TObjArray* objarray = s_input.Tokenize( delim );
    Int_t substringN = objarray->GetEntries();
    vector<const char*> substrings;



    for( Int_t i = 0; i < substringN; i++ )
    {
        TString tmp =  ( (TObjString*)objarray->At(i) ) -> GetString();

        substrings.push_back( Form( "%s", tmp.Data() )  );

    }

    return substrings;
}



// update the customized bg value
void Dlg_Change_histo_X_Range::Update_marker( ){

        if( fEntry_out != "" )
        {

            vector<const char*> ranges = ToParseString( fEntry_out, " " );

            if( ranges.size() >= 2) {

                TString s1 = ranges[0] ;
                TString s2 = ranges[1] ;

                *fXup_select  = TMath::Max(   s1.Atof(),   s2.Atof() );
                *fXlow_select = TMath::Min(   s1.Atof(),   s2.Atof() );

                CloseWindow();
            } else {
                // do nothing.
            }


        }

}



void Dlg_Change_histo_X_Range::Cancel(){

    // return to original value.
    *fXup_select  = fXup_backup;
    *fXlow_select = fXlow_backup;
    CloseWindow();


 }


void Dlg_Change_histo_X_Range::CloseWindow(){


    fBtn_close->SetState(kButtonDisabled); // to avoid double click.
    fBtn_cancel->SetState(kButtonDisabled);
    DeleteWindow();


 }


void Dlg_Change_histo_X_Range::To_focus_on_entry( Event_t* e) {

    fTextEntries->SetFocus();
    fTextEntries->SelectAll();
}



void Dlg_Change_histo_X_Range::To_response_key(Event_t* e) {

    if ( e->fType == kGKeyPress   )
    {
        // using LookuString to get the key mapping.
        UInt_t key_symbol;
        char tmp[2];
        gVirtualX->LookupString( e, tmp,sizeof(tmp), key_symbol );

         const unsigned key_enter = 36;

        if( key_symbol == kKey_Enter   ) {  Update_marker(); } // the enter key near num pad

        else if ( e->fCode == key_enter ) {  Update_marker(); } // the enter key in regular position.

        else if( key_symbol == kKey_Escape ) {  CloseWindow(); }

    }

}


// class constructor
Dlg_Change_histo_X_Range::Dlg_Change_histo_X_Range( const TGWindow* p,
                                UInt_t w,
                                UInt_t h,
                                float  primitive_xmin,
                                float  primitive_xmax,
                                float* user_min,
                                float* user_max )
: TGMainFrame(p, w, h)
{

    fXup_select  = user_max;
    fXlow_select = user_min;
    fXup_backup  = primitive_xmax;
    fXlow_backup = primitive_xmin;

    *fXup_select  = fXup_backup;  // default value.
    *fXlow_select = fXlow_backup; //default value.

    // for user know the range
    TString label_text = Form( " [%.1f to %.1f] ", primitive_xmin, primitive_xmax );



    SetCleanup(kDeepCleanup); // important step for closing windows properly.

    TGLayoutHints*  Layout1 = new TGLayoutHints( kLHintsCenterY, 2, 2, 2, 2);


    //-------------------------------------------------------------------| Label-Entry pair

    fHF = new TGHorizontalFrame( this , 200, 30);

    AddFrame( fHF, Layout1 );


    fLabel = new TGLabel( fHF, label_text.Data() );  /* base, text */

    fTextEntries = new TGTextEntry(  fHF, "", 1);   /* base, inital txt, ID */




    fHF->AddFrame( fTextEntries, Layout1 );
    fHF->AddFrame( fLabel, Layout1 );





    //------------------------------------------------------------------| Connect
    fTextEntries
        -> Connect( "TextChanged(char*)",
                    "Dlg_Change_histo_X_Range", this, "Update_entry_output(char*)");

    fTextEntries
        -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_Change_histo_X_Range", this,
                    "To_response_key(Event_t*)");


    TGLayoutHints*  Layout2 = new TGLayoutHints( kLHintsExpandX, 2, 2, 2, 2);
    fHF_btns = new TGHorizontalFrame( this , 200, 30);

    //-------------------------------------------------------------------| Btn for close the window
    fBtn_close = new TGTextButton( fHF_btns, "OK" );
    fBtn_close -> Connect( "Clicked()", "Dlg_Change_histo_X_Range", this, "CloseWindow()"  );
    fHF_btns->AddFrame( fBtn_close, Layout2);

    //-------------------------------------------------------------------| Btn for cancel the change

    fBtn_cancel = new TGTextButton( fHF_btns, "Cancel" );
    fBtn_cancel -> Connect( "Clicked()", "Dlg_Change_histo_X_Range", this, "Cancel()"  );
    fHF_btns->AddFrame( fBtn_cancel, Layout2);


    AddFrame( fHF_btns, Layout2);



    //-----------------------------------------------------------to focus on entry
    this->Connect(  "ProcessedEvent(Event_t*)", "Dlg_Change_histo_X_Range", this,
                    "To_focus_on_entry(Event_t*)" );



    SetName("Dlg_Change_histo_X_Range");
    SetWindowName("Change histo x range");


    MapSubwindows();
    Resize( GetDefaultSize() );
    MapWindow();

    gClient->WaitFor(this);

}





////////////////////////////////////////////////////////////////////////////////


class Dlg_projection_subtractMode: public TGMainFrame {

    Float_t*  fAddGate_1;
    Float_t*  fAddGate_2;
    Float_t*  fSubGate_1;
    Float_t*  fSubGate_2;
    Float_t   fBackupValues[4];
    TString*  fProjDirection;

    TGHorizontalFrame*  fHF[5];
    TGHorizontalFrame*  fHF_btns;


    TString             fString[5];
    TGLabel*            fLabels[5];
    TGNumberEntry*      fEntry[4];
    TGTextEntry*        fTextEty;

    Bool_t  firstTime;
    Bool_t*  fDoProj;
    Int_t   tabSwitch;

    TGTextButton*       fBtn_close;
    TGTextButton*       fBtn_cancel;

public:

    // class constructor
   Dlg_projection_subtractMode(     const TGWindow* p,
                                    Float_t*  addGate1,
                                    Float_t*  addGate2,
                                    Float_t*  subGate1,
                                    Float_t*  subGate2,
                                    TString*  projDirection,
                                    Bool_t*   doProj  );

    ~Dlg_projection_subtractMode();

    void    To_switch_tab();

    void    To_response_key( Event_t* );

    void    To_response_key2( Event_t* );

    void    To_focus_on_entry( Event_t* );

    void    Update_proj(char*);

    void    Cancel();

    void    CloseWindow();

    void    Update_values();

};


void Dlg_projection_subtractMode::Update_proj( char* entry_output) {

    *fProjDirection = entry_output;

}



void Dlg_projection_subtractMode::To_switch_tab(){


    if( tabSwitch%5 ==0) { tabSwitch=0; }
    if( tabSwitch == 0 ) {
        fEntry[0]->GetNumberEntry()->SetFocus();
        fEntry[0]->GetNumberEntry()->SelectAll();
    }
    else if( tabSwitch == 1 ) {
        fEntry[1]->GetNumberEntry()->SetFocus();
        fEntry[1]->GetNumberEntry()->SelectAll();
    }
    else if( tabSwitch == 2 ) {
        fEntry[2]->GetNumberEntry()->SetFocus();
        fEntry[2]->GetNumberEntry()->SelectAll();
    }
    else if( tabSwitch == 3 ) {
        fEntry[3]->GetNumberEntry()->SetFocus();
        fEntry[3]->GetNumberEntry()->SelectAll();
    }
    else if( tabSwitch == 4 ) {
        fTextEty->SelectAll();
        fTextEty->SetFocus();
    }


}



void Dlg_projection_subtractMode::Update_values( ){

    *fAddGate_1    = fEntry[0]->GetNumber();
    *fAddGate_2    = fEntry[1]->GetNumber();
    *fSubGate_1    = fEntry[2]->GetNumber();
    *fSubGate_2    = fEntry[3]->GetNumber();
    *fProjDirection = fTextEty -> GetText();
    CloseWindow();

}



void Dlg_projection_subtractMode::Cancel(){


    *fAddGate_1    = fBackupValues[0];
    *fAddGate_2    = fBackupValues[1];
    *fSubGate_1    = fBackupValues[2];
    *fSubGate_2    = fBackupValues[3];

    CloseWindow();

    *fDoProj = false;
 }


void Dlg_projection_subtractMode::CloseWindow(){

    fBtn_close->SetState(kButtonDisabled); // to avoid double click.
    fBtn_cancel->SetState(kButtonDisabled);
    DeleteWindow();

 }


void Dlg_projection_subtractMode::To_focus_on_entry( Event_t* e) {

        fTextEty->SelectAll();
        fTextEty->SetFocus();
}


void Dlg_projection_subtractMode::To_response_key(Event_t* e) {

    /* using space as the hotkey to finish the input. */


    // for key symbol look up.
    UInt_t key_symbol;
    char tmp[2];
    if ( e->fType == kGKeyPress )
    {  gVirtualX->LookupString( e, tmp,sizeof(tmp), key_symbol ); }

    if ( e->fType == kGKeyPress  )
    {

        if( key_symbol == kKey_Tab   ) { tabSwitch++; To_switch_tab(); }

        else if( key_symbol ==  kKey_Backtab ) { tabSwitch--; To_switch_tab(); }

        //else if( key_symbol == kKey_Space  )   { Update_values(); }
    }



}





void Dlg_projection_subtractMode::To_response_key2(Event_t* e) {

    /* using space as the hotkey to finish the input. */


    // for key symbol look up.
    UInt_t key_symbol;
    char tmp[2];
    if ( e->fType == kGKeyPress )
    {  gVirtualX->LookupString( e, tmp,sizeof(tmp), key_symbol ); }

    if ( e->fType == kGKeyPress  )
    {
        const unsigned key_enter = 36;

        if( key_symbol == kKey_Tab   ) { tabSwitch++; To_switch_tab(); }

        else if( key_symbol ==  kKey_Backtab ) { tabSwitch--; To_switch_tab(); }

        else if ( key_symbol == kKey_Enter   ) { Update_values(); }

        else if ( e->fCode == key_enter ) { Update_values(); }
    }



}




// class constructor
Dlg_projection_subtractMode::Dlg_projection_subtractMode( const TGWindow* p,
                                        Float_t*  addGate1,
                                        Float_t*  addGate2,
                                        Float_t*  subGate1,
                                        Float_t*  subGate2,
                                        TString*  projDirection,
                                        Bool_t*   doProj    )
: TGMainFrame(p, 10, 10)
{
    fDoProj         = doProj;
    *fDoProj        = true;
    firstTime       = true;
    tabSwitch       = 0;
    fAddGate_1      = addGate1;
    fAddGate_2      = addGate2;
    fSubGate_1      = subGate1;
    fSubGate_2      = subGate2;
    fProjDirection  = projDirection;



    SetCleanup(kDeepCleanup); // important step for closing windows properly.
    TGLayoutHints*  Layout1 = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 2, 2, 2, 2);

    fString[0] = Form( "add gate range 1");
    fString[1] = Form( "add gate range 2");
    fString[2] = Form( "subtract gate range 1");
    fString[3] = Form( "subtract gate range 2");
    fString[4] = Form( "proj X or Y");



    float init_value[4]
    = { *addGate1, *addGate2, *subGate1, *subGate2 };
    for(Int_t i =0; i<4; i++) {   fBackupValues[i] = init_value[i]; }


    for( Int_t i=0; i<5; i++){

        fHF[i]    = new TGHorizontalFrame( this , 10, 10);
        AddFrame( fHF[i], Layout1 );

        if( i<4){
             fEntry[i]
            = new TGNumberEntry(    fHF[i],         /* base */
                                    init_value[i],  /* inital value */
                                    12,             /* digit width */
                                    i,              /* ID */
                                    TGNumberFormat::kNESRealTwo);
        fEntry[i]->GetNumberEntry()
        -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_projection_subtractMode", this,
                    "To_response_key(Event_t*)");

        } else {
             fTextEty
            = new TGTextEntry(  fHF[i], "", 1); /* base, init value, ID */

            fTextEty
            -> Connect( "TextChanged(char*)",
                    "Dlg_projection_subtractMode", this, "Update_proj(char*)");

            fTextEty
            -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_projection_subtractMode", this,
                    "To_response_key2(Event_t*)");
        }




        fLabels[i] = new TGLabel( fHF[i], fString[i].Data() );
        fLabels[i]->SetTextJustify( kTextCenterX );
        fHF[i]->AddFrame( fEntry[i], Layout1 );
        fHF[i]->AddFrame( fLabels[i], Layout1 );
    }




    TGLayoutHints*  Layout2 = new TGLayoutHints( kLHintsExpandX, 2, 2, 2, 2);
    fHF_btns = new TGHorizontalFrame( this , 200, 30);

    //---------------------------------------------------------| Btn for close the window

    fBtn_close = new TGTextButton( fHF_btns, "OK" );
    fBtn_close -> Connect( "Clicked()", "Dlg_projection_subtractMode", this, "Update_values()"  );
    fHF_btns->AddFrame( fBtn_close, Layout2);

    //---------------------------------------------------------| Btn for cancel the change

    fBtn_cancel = new TGTextButton( fHF_btns, "Cancel" );
    fBtn_cancel -> Connect( "Clicked()", "Dlg_projection_subtractMode", this, "Cancel()"  );
    fHF_btns->AddFrame( fBtn_cancel, Layout2);


    AddFrame( fHF_btns, Layout2);



    //-----------------------------------------------------------to focus on entry

    this->Connect(  "ProcessedEvent(Event_t*)", "Dlg_projection_subtractMode", this,
                    "To_focus_on_entry(Event_t*)" );


    SetName("Dlg_projection_subtractMode");
    SetWindowName("project with substract mode");


    MapSubwindows();
    Resize( GetDefaultSize() );
    MapWindow();

    gClient->WaitFor(this);

}

Dlg_projection_subtractMode::~Dlg_projection_subtractMode() {

    DeleteWindow();

}




///////////////////////////////////////////////////////////////////////////////


class Dlg_double_gaussian: public TGMainFrame {

    float*  fPeak1_h;
    float*  fPeak2_h;

    float*  fPeak1_center;
    float*  fPeak2_center;

    float*  fPeak1_sigma;
    float*  fPeak2_sigma;

    Bool_t  firstTime;
    Int_t   tabSwitch;


    TGHorizontalFrame*  fHF_peak1[3];
    TGHorizontalFrame*  fHF_peak2[3];
    TGHorizontalFrame*  fHF_separate;
    TGHorizontalFrame*  fHF_btns;

    TGHorizontal3DLine* fAline;

    TString             fPeak1String[3];
    TString             fPeak2String[3];


    TGLabel*            fLabel_peak1[3];
    TGLabel*            fLabel_peak2[3];


    TGNumberEntry*      fEntry_peak1[3];
    TGNumberEntry*      fEntry_peak2[3];


    TGTextButton*       fBtn_close;
    TGTextButton*       fBtn_cancel;



public:

    // class constructor
   Dlg_double_gaussian( const TGWindow* p,
                        float*  peak1_h,
                        float*  peak1_center,
                        float*  peak1_sigma,
                        float*  peak2_h,
                        float*  peak2_center,
                        float*  peak2_sigma );

    void    To_switch_tab();

    void    To_response_key( Event_t* );

    void    To_focus_on_entry( Event_t* );

    void    Cancel();

    void    CloseWindow();

    void    Update_values();

};


void Dlg_double_gaussian::To_switch_tab(){


    if( tabSwitch%6 ==0) { tabSwitch=0; }
    if( tabSwitch == 0 ) {
        fEntry_peak1[0]->GetNumberEntry()->SetFocus();
        fEntry_peak1[0]->GetNumberEntry()->SelectAll();
    }
    else if( tabSwitch == 1 ) {
        fEntry_peak1[1]->GetNumberEntry()->SetFocus();
        fEntry_peak1[1]->GetNumberEntry()->SelectAll();
    }
    else if( tabSwitch == 2 ) {
        fEntry_peak1[2]->GetNumberEntry()->SetFocus();
        fEntry_peak1[2]->GetNumberEntry()->SelectAll();
    }
    else if( tabSwitch == 3 ) {
        fEntry_peak2[0]->GetNumberEntry()->SetFocus();
        fEntry_peak2[0]->GetNumberEntry()->SelectAll();
    }
    else if( tabSwitch == 4 ) {
        fEntry_peak2[1]->GetNumberEntry()->SetFocus();
        fEntry_peak2[1]->GetNumberEntry()->SelectAll();
    }
    else if( tabSwitch == 5 ) {
        fEntry_peak2[2]->GetNumberEntry()->SetFocus();
        fEntry_peak2[2]->GetNumberEntry()->SelectAll();
    }



}



void Dlg_double_gaussian::Update_values( ){

    *fPeak1_h        = fEntry_peak1[0]->GetNumber();
    *fPeak1_center   = fEntry_peak1[1]->GetNumber();
    *fPeak1_sigma    = fEntry_peak1[2]->GetNumber();
    *fPeak2_h        = fEntry_peak2[0]->GetNumber();
    *fPeak2_center   = fEntry_peak2[1]->GetNumber();
    *fPeak2_sigma    = fEntry_peak2[2]->GetNumber();
    CloseWindow();

}



void Dlg_double_gaussian::Cancel(){

    *fPeak1_h        = 0;
    *fPeak1_center   = 0;
    *fPeak1_sigma    = 0;
    *fPeak2_h        = 0;
    *fPeak2_center   = 0;
    *fPeak2_sigma    = 0;
    CloseWindow();


 }


void Dlg_double_gaussian::CloseWindow(){

    fBtn_close->SetState(kButtonDisabled); // to avoid double click.
    fBtn_cancel->SetState(kButtonDisabled);
    DeleteWindow();

 }


void Dlg_double_gaussian::To_focus_on_entry( Event_t* e) {

    // seeming not working well.. it will frozen...
//    if( firstTime ) {
//        fEntry_peak1[0]->GetNumberEntry()->SelectAll();
//        fEntry_peak1[0]->GetNumberEntry()->SetFocus();
//
//        firstTime = false;
//    }
}


void Dlg_double_gaussian::To_response_key(Event_t* e) {

    /* using space as the hotkey to finish the input. */


    // for key symbol look up.
    UInt_t key_symbol;
    char tmp[2];
    if ( e->fType == kGKeyPress )
    {  gVirtualX->LookupString( e, tmp,sizeof(tmp), key_symbol ); }

    if ( e->fType == kGKeyPress  )
    {

        if( key_symbol == kKey_Tab   ) { tabSwitch++; To_switch_tab(); }

        else if( key_symbol ==  kKey_Backtab ) { tabSwitch--; To_switch_tab(); }

        else if( key_symbol == kKey_Space  )   { Update_values(); }
    }



}




// class constructor
Dlg_double_gaussian::Dlg_double_gaussian(   const TGWindow* p,
                                            float*  peak1_h,
                                            float*  peak1_center,
                                            float*  peak1_sigma,
                                            float*  peak2_h,
                                            float*  peak2_center,
                                            float*  peak2_sigma  )
: TGMainFrame(p, 10, 10)
{

    firstTime       = true;
    tabSwitch       = 0;
    fPeak1_h        = peak1_h;
    fPeak1_center   = peak1_center;
    fPeak1_sigma    = peak1_sigma;
    fPeak2_h        = peak2_h;
    fPeak2_center   = peak2_center;
    fPeak2_sigma    = peak2_sigma;



    SetCleanup(kDeepCleanup); // important step for closing windows properly.

    TGLayoutHints*  Layout1 = new TGLayoutHints( kLHintsCenterY, 2, 2, 2, 2);




        fPeak1String[0] = Form( "peak1 height");
        fPeak2String[0] = Form( "peak2 height");

        fPeak1String[1] = Form( "peak1 center");
        fPeak2String[1] = Form( "peak2 center");

        fPeak1String[2] = Form( "peak1 width ");
        fPeak2String[2] = Form( "peak2 width ");




    //-------------------------------------------------------------------| peak1

    float init_value1[3] = { *fPeak1_h, *fPeak1_center, *fPeak1_sigma };


    for( Int_t i =0; i<3; i++){

        fHF_peak1[i] = new TGHorizontalFrame( this , 200, 30);

        fLabel_peak1[i] = new TGLabel( fHF_peak1[i], fPeak1String[i].Data() );

        fEntry_peak1[i]
        = new TGNumberEntry(    fHF_peak1[i],   /* base */
                                init_value1[i], /* inital value */
                                12,             /* digit width */
                                i,              /* ID */
                                TGNumberFormat::kNESRealOne);

        fEntry_peak1[i]->GetNumberEntry()
        -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_double_gaussian", this,
                    "To_response_key(Event_t*)");

        fLabel_peak1[i]->SetTextJustify( kTextCenterX );
        fEntry_peak1[i]->Resize(150, fEntry_peak1[i]->GetDefaultHeight() );

        fHF_peak1[i]->AddFrame( fLabel_peak1[i], Layout1 );
        fHF_peak1[i]->AddFrame( fEntry_peak1[i], Layout1 );
        AddFrame( fHF_peak1[i], Layout1 );
    }


    //================================= separation line
    fHF_separate =  new TGHorizontalFrame( this , 200, 30);
    fAline = new TGHorizontal3DLine( fHF_separate, 250  );
    fHF_separate->AddFrame( fAline, Layout1 );
    AddFrame( fHF_separate, Layout1 );


    //-------------------------------------------------------------------| peak2

    float init_value2[3] = { *fPeak2_h, *fPeak2_center, *fPeak2_sigma };

    for( Int_t i =0; i<3; i++){

        fHF_peak2[i] = new TGHorizontalFrame( this , 200, 30);

        fLabel_peak2[i] = new TGLabel( fHF_peak2[i], fPeak2String[i].Data() );

        fEntry_peak2[i]
        = new TGNumberEntry(    fHF_peak2[i],   /* base */
                                init_value2[i], /* inital value */
                                12,             /* digit width */
                                i+3,            /* ID */
                                TGNumberFormat::kNESRealOne);


        fEntry_peak2[i]->GetNumberEntry()
        -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_double_gaussian", this,
                    "To_response_key(Event_t*)");


        fLabel_peak2[i]->SetTextJustify( kTextCenterX );
        fEntry_peak2[i]->Resize(150, fEntry_peak2[i]->GetDefaultHeight() );

        fHF_peak2[i]->AddFrame( fLabel_peak2[i], Layout1 );
        fHF_peak2[i]->AddFrame( fEntry_peak2[i], Layout1 );
        AddFrame( fHF_peak2[i], Layout1 );
    }




    TGLayoutHints*  Layout2 = new TGLayoutHints( kLHintsExpandX, 2, 2, 2, 2);
    fHF_btns = new TGHorizontalFrame( this , 200, 30);

    //---------------------------------------------------------| Btn for close the window

    fBtn_close = new TGTextButton( fHF_btns, "OK" );
    fBtn_close -> Connect( "Clicked()", "Dlg_double_gaussian", this, "Update_values()"  );
    fHF_btns->AddFrame( fBtn_close, Layout2);

    //---------------------------------------------------------| Btn for cancel the change
    // all h,c,sigma are set to zero.
    fBtn_cancel = new TGTextButton( fHF_btns, "Cancel" );
    fBtn_cancel -> Connect( "Clicked()", "Dlg_double_gaussian", this, "Cancel()"  );
    fHF_btns->AddFrame( fBtn_cancel, Layout2);


    AddFrame( fHF_btns, Layout2);



    //-----------------------------------------------------------to focus on entry
    // seeming not working well...
//    this->Connect(  "ProcessedEvent(Event_t*)", "Dlg_double_gaussian", this,
//                    "To_focus_on_entry(Event_t*)" );


    SetName("Dlg_double_gaussian");
    SetWindowName("Double Gaussian fit");


    MapSubwindows();
    Resize( GetDefaultSize() );
    MapWindow();

    gClient->WaitFor(this);

}


///////////////////////////////////////////////////////////////////////////////







class Dlg_to_find_peaks: public TGMainFrame {

    Float_t*  fSigma_guess;
    Float_t*  fThreshold;

    Float_t*  fText_size;
    Float_t*  fText_precision;
    Float_t   fBackupValues[4];

    Bool_t  firstTime;
    Int_t   tabSwitch;


    TGHorizontalFrame*  fHF[4];

    TGHorizontalFrame*  fHF_btns;



    TString             fString[4];
    TGLabel*            fLabels[4];
    TGNumberEntry*      fEntry[4];

    TGTextButton*       fBtn_close;
    TGTextButton*       fBtn_cancel;

public:

    // class constructor
   Dlg_to_find_peaks(   const TGWindow* p,
                        Float_t*  sigma_guess,
                        Float_t*  threshold,
                        Float_t*  text_size,
                        Float_t*  text_precision   );

    ~Dlg_to_find_peaks();

    void    To_switch_tab();

    void    To_response_key( Event_t* );

    void    To_focus_on_entry( Event_t* );

    void    Cancel();

    void    CloseWindow();

    void    Update_values();

};


void Dlg_to_find_peaks::To_switch_tab(){


    if( tabSwitch%4 ==0) { tabSwitch=0; }
    if( tabSwitch == 0 ) {
        fEntry[0]->GetNumberEntry()->SetFocus();
        fEntry[0]->GetNumberEntry()->SelectAll();
    }
    else if( tabSwitch == 1 ) {
        fEntry[1]->GetNumberEntry()->SetFocus();
        fEntry[1]->GetNumberEntry()->SelectAll();
    }
    else if( tabSwitch == 2 ) {
        fEntry[2]->GetNumberEntry()->SetFocus();
        fEntry[2]->GetNumberEntry()->SelectAll();
    }
    else if( tabSwitch == 3 ) {
        fEntry[3]->GetNumberEntry()->SetFocus();
        fEntry[3]->GetNumberEntry()->SelectAll();
    }


}



void Dlg_to_find_peaks::Update_values( ){

    *fSigma_guess       = fEntry[0]->GetNumber();
    *fThreshold         = fEntry[1]->GetNumber();
    *fText_size         = fEntry[2]->GetNumber();
    *fText_precision    = fEntry[3]->GetNumber();

    CloseWindow();

}



void Dlg_to_find_peaks::Cancel(){

    *fSigma_guess       = fBackupValues[0];
    *fThreshold         = fBackupValues[1];
    *fText_size         = fBackupValues[2];
    *fText_precision    = fBackupValues[3];

    CloseWindow();


 }


void Dlg_to_find_peaks::CloseWindow(){

    fBtn_close->SetState(kButtonDisabled); // to avoid double click.
    fBtn_cancel->SetState(kButtonDisabled);
    DeleteWindow();

 }


void Dlg_to_find_peaks::To_focus_on_entry( Event_t* e) {

    // seeming not working well.. it will frozen...
//    if( firstTime ) {
//        fEntry_peak1[0]->GetNumberEntry()->SelectAll();
//        fEntry_peak1[0]->GetNumberEntry()->SetFocus();
//
//        firstTime = false;
//    }
}


void Dlg_to_find_peaks::To_response_key(Event_t* e) {

    /* using space as the hotkey to finish the input. */


    // for key symbol look up.
    UInt_t key_symbol;
    char tmp[2];
    if ( e->fType == kGKeyPress )
    {  gVirtualX->LookupString( e, tmp,sizeof(tmp), key_symbol ); }

    if ( e->fType == kGKeyPress  )
    {

        if( key_symbol == kKey_Tab   ) { tabSwitch++; To_switch_tab(); }

        else if( key_symbol ==  kKey_Backtab ) { tabSwitch--; To_switch_tab(); }

        else if( key_symbol == kKey_Space  )   { Update_values(); }
    }



}




// class constructor
Dlg_to_find_peaks::Dlg_to_find_peaks(   const TGWindow* p,
                                        Float_t*  sigma_guess,
                                        Float_t*  threshold,
                                        Float_t*  text_size,
                                        Float_t*  text_precision  )
: TGMainFrame(p, 10, 10)
{
    firstTime       = true;
    tabSwitch       = 0;
    fSigma_guess    = sigma_guess;
    fThreshold      = threshold;
    fText_size      = text_size;
    fText_precision = text_precision;




    SetCleanup(kDeepCleanup); // important step for closing windows properly.
    TGLayoutHints*  Layout1 = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 2, 2, 2, 2);

    fString[0] = Form( "min sigma value");
    fString[1] = Form( "threshold (0-1)" );
    fString[2] = Form( "text label size");
    fString[3] = Form( "demical point");



    float init_value[4]
    = { *fSigma_guess, *fThreshold, *fText_size, *fText_precision };
    for(Int_t i =0; i<4; i++) {   fBackupValues[i] = init_value[i]; }


    for( Int_t i=0; i<4; i++){

        fHF[i]    = new TGHorizontalFrame( this , 10, 10);
        AddFrame( fHF[i], Layout1 );

        if( i<2){
             fEntry[i]
            = new TGNumberEntry(    fHF[i],         /* base */
                                    init_value[i],  /* inital value */
                                    12,             /* digit width */
                                    i,              /* ID */
                                    TGNumberFormat::kNESRealTwo);
        } else {
             fEntry[i]
            = new TGNumberEntry(    fHF[i],         /* base */
                                    init_value[i],  /* inital value */
                                    12,             /* digit width */
                                    i,              /* ID */
                                    TGNumberFormat::  kNESInteger );
        }

        fEntry[i]->GetNumberEntry()
        -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_to_find_peaks", this,
                    "To_response_key(Event_t*)");



        fLabels[i] = new TGLabel( fHF[i], fString[i].Data() );
        fLabels[i]->SetTextJustify( kTextCenterX );
        fHF[i]->AddFrame( fEntry[i], Layout1 );
        fHF[i]->AddFrame( fLabels[i], Layout1 );
    }




    TGLayoutHints*  Layout2 = new TGLayoutHints( kLHintsExpandX, 2, 2, 2, 2);
    fHF_btns = new TGHorizontalFrame( this , 200, 30);

    //---------------------------------------------------------| Btn for close the window

    fBtn_close = new TGTextButton( fHF_btns, "OK" );
    fBtn_close -> Connect( "Clicked()", "Dlg_to_find_peaks", this, "Update_values()"  );
    fHF_btns->AddFrame( fBtn_close, Layout2);

    //---------------------------------------------------------| Btn for cancel the change

    fBtn_cancel = new TGTextButton( fHF_btns, "Cancel" );
    fBtn_cancel -> Connect( "Clicked()", "Dlg_to_find_peaks", this, "Cancel()"  );
    fHF_btns->AddFrame( fBtn_cancel, Layout2);


    AddFrame( fHF_btns, Layout2);



    //-----------------------------------------------------------to focus on entry
    // seeming not working well...
//    this->Connect(  "ProcessedEvent(Event_t*)", "Dlg_to_find_peaks", this,
//                    "To_focus_on_entry(Event_t*)" );


    SetName("Dlg_to_find_peaks");
    SetWindowName("Finding peaks setup");


    MapSubwindows();
    Resize( GetDefaultSize() );
    MapWindow();

    gClient->WaitFor(this);

}

Dlg_to_find_peaks::~Dlg_to_find_peaks() {

    DeleteWindow();

}

///////////////////////////////////////////////////////////////////////////////


/*

    A dialog for histogram operations, for example:
    if user input " 99 = 1 + 2"
    then we create a histogram from adding histos[0] + histos[1]

    vector<TH1*>  histos is from input.
    here, we will both update the open_list, and histos.

*/
class Dlg_Operation_histos: public TGMainFrame {

private:

    TH1*             fHisto;
    vector<TH1*>*    fHistos;
    TString*        fHisto_list;
    TString*        fMessage;
    TString         fHisto_list_backup;
    TString         fEntry_out;

    TGVerticalFrame*    fVF;
    TGHorizontalFrame*  fHF_1;
    TGHorizontalFrame*  fHF_2;
    TGVerticalFrame*    fHisto_frame;
    TGVerticalFrame*    fHisto_frame2;
    TGLabel*	        fHisto_nameLabel[200]; // set a upper limit = 200



    TGLabel*            fLabel;
    TGTextEntry*        fTextEntries;
    TGTextButton*       fBtn_close;
    TGTextButton*       fBtn_cancel;



public:

    // class constructor
    Dlg_Operation_histos(
        const TGWindow* ,       /* base windows */
        UInt_t ,                /* width */
        UInt_t ,                /* height */
        TString* histo_list,   /* value passing by reference */
        vector<TH1*>*  histos,
        TH1*           histo,
        TString* message  );

    void    To_response_key( Event_t* );

    void    To_focus_on_entry( Event_t* );

    void    CloseWindow();

    void    Cancel();

    void    Parse_expression( );

    void    Update_entry_output(char*);

    bool    Special_case( TString epr );

    void    General_case( TString epr );

    void    combo_case( TString epr );

    void    del_spectra( TString epr );

    void    to_display_histos( TString epr);

    void    to_apply_formula( TString epr);
};



void Dlg_Operation_histos::Cancel(){

    // cancel the change, and use the original setting.
    *fHisto_list = fHisto_list_backup.Data();
    CloseWindow();
 }


void Dlg_Operation_histos::CloseWindow(){

    fBtn_close->SetState(kButtonDisabled); // to avoid double click.
    fBtn_cancel->SetState(kButtonDisabled);

    DeleteWindow();
 }



void Dlg_Operation_histos::To_focus_on_entry( Event_t* e) {

    fTextEntries->SetFocus();
    fTextEntries->SelectAll();
}


void Dlg_Operation_histos::To_response_key(Event_t* e) {

    if ( e->fType == kGKeyPress   )
    {
        // using LookuString to get the key mapping.
        UInt_t key_symbol;
        char tmp[2];
        gVirtualX->LookupString( e, tmp,sizeof(tmp), key_symbol );

         const unsigned key_enter = 36;

        if( key_symbol == kKey_Enter   )  {  Parse_expression(); }
        // the enter key near num pad

        else if ( e->fCode == key_enter ) {  Parse_expression(); }
        // the enter key in regular position.

        else if( key_symbol == kKey_Escape   )  {  CloseWindow(); }

    }

}


void Dlg_Operation_histos::Update_entry_output( char* entry_output ) {

    fEntry_out = entry_output;
}

void Dlg_Operation_histos::to_apply_formula( TString epr ){


    //============================================
    // parse epr and create formula
    // epr will be
    // " let x = x + 1 " subject = x, formula = x + 1


    TObjArray* tmp_array;
    tmp_array= epr.Tokenize( "=" );
    int sizeN = tmp_array->GetEntries();

    TString formula =  ( (TObjString*)tmp_array->At( sizeN-1) ) -> GetString();




    //============================================
    // to adjust the TH1 according to formula
    //
    int binxN = fHisto->GetXaxis()->GetNbins();
    unique_ptr<TH1> histo_copy(  (TH1*) fHisto->Clone() );



    //  formula ex. x = x + 1
    unique_ptr<TF1> func( new TF1("func", formula.Data() ) );

    if( func->IsValid() )
    {
        *fMessage = "apply formula";
        fHisto->Reset();
        for( int ix=1; ix<=binxN; ++ix ) {

            int counts = histo_copy->GetBinContent(ix);
            if( counts == 0 ) continue;

            double x = fHisto->GetXaxis()->GetBinCenter( ix );
            double xNew  = func->Eval( x );
            int    ixNew = fHisto->GetXaxis()->FindBin( xNew );
            fHisto->SetBinContent( ixNew,  counts );

        }
    }



}

void Dlg_Operation_histos::del_spectra( TString epr ){

    // del 1..3  means to delete h1, h2, and h3 ( continuous range )
    // del 1 5 7 means to delete h1, h5, and h7

    Int_t    idx1stDot = -1;
    Int_t    idx2ndDot = -1;
    Int_t    idxDel = epr.Index("l") + 1;
    TString  s2 = epr.SubString( ".." );
    if( s2 != "" ) { idx1stDot = epr.Index(".");  idx2ndDot = idx1stDot + 1; }

    // STEP 2 Get operands
    TString tmp;

    Int_t   hIdxRange1 =  0;
    Int_t   hIdxRange2 =  0;
    Bool_t   check1;
    Bool_t   check2;

    Bool_t  IsValidEpr  = false;
    if(  s2 != "" ) {

        tmp = epr( idxDel,idx1stDot);
        hIdxRange1 = tmp.Atoi();
        hIdxRange1--; // since in the panel, we label histo[0] as spec no.1

        tmp = epr(idx2ndDot+1, epr.Length() );
        hIdxRange2 = tmp.Atoi();
        hIdxRange2--;

        check1 = ( hIdxRange1>=0 ) && ( hIdxRange1< fHistos->size() );
        check2 = ( hIdxRange2>=hIdxRange1 ) && ( hIdxRange2< fHistos->size() );

        if( check1 && check2 ) IsValidEpr = true;
    }

    if( IsValidEpr ) {

        // delete spectra
        Int_t del_operation_N = hIdxRange2 - hIdxRange1 + 1;
        Int_t starting_idx = hIdxRange1;
        for( Int_t i=0; i<del_operation_N; i++ ) {

            // make sure we have at least one histogram.
            if( fHistos->size() >= 2 ){

                fHistos->erase( fHistos->begin() + starting_idx );

            }

        }

        *fMessage = Form("delte spec from %d to %d", hIdxRange1+1,hIdxRange2+1 ) ;

    }




    if( s2=="" ){

        TObjArray* objarray = epr.Tokenize(" "); // separate by space
        Int_t substringN = objarray->GetEntries();
        vector<Int_t> idx_to_del;
        vector<Int_t> idx_to_del_temp;


        for( Int_t i = 1; i < substringN; i++ ) {

            TString tmp =  ( (TObjString*)objarray->At(i) ) -> GetString();

            Int_t idx = tmp.Atoi() - 1; // note: we label histo[0] as spec no.1

            if( idx < fHistos->size() ) {

                idx_to_del_temp.push_back( idx );

            }
        }

        // to sort idx_to_del
        while( 1 ){

            Int_t min_idx = 999;
            Int_t location = -1;

            for( Int_t i = 0; i<idx_to_del_temp.size(); i++ )
            {
                if( idx_to_del_temp.at(i) < min_idx ) {

                    min_idx = idx_to_del_temp.at(i);
                    location = i;
                }
            }

            idx_to_del.push_back( min_idx );
            idx_to_del_temp.erase( idx_to_del_temp.begin() + location );
            if( idx_to_del_temp.size() == 0 ){ break; }

        } // ---- end of while loop.


        // now we erase the selected histogram.
        Int_t starting_idx;
        TString msg = "";
        for( Int_t i = 0; i<idx_to_del.size(); i++ ){

            msg += Form("%d ", idx_to_del.at(i) + 1 );

            starting_idx = idx_to_del.at(i) - i;

            fHistos->erase( fHistos->begin() + starting_idx );

        }

         *fMessage = Form("delte spec %s", msg.Data() ) ;
    }

}


void Dlg_Operation_histos::combo_case( TString epr ) {

    // current only apply for: 99 += *
    // in the future         : 99 += 1..3 means h99 = h1 + h2 + h3

    // checking we having "+=" operator
    Bool_t      hasPlusEqualSign = false;
    TObjArray* objarray;
    objarray = epr.Tokenize( "+=" );

    Int_t    idxEqual= epr.Index("=");
    Int_t    idxPlus = epr.Index("+");
    Int_t    idxAsterisk = epr.Index("*");
    Int_t    idx1stDot = -1;
    Int_t    idx2ndDot = -1;
    TString  s2 = epr.SubString( ".." );
    if( s2 != "" ) { idx1stDot = epr.Index(".");  idx2ndDot = idx1stDot + 1; }


    Bool_t   check1 = ( (idxEqual-idxPlus) == 1 );
    Bool_t   check2 = ( idxEqual != -1 );
    Bool_t   check3 = ( idxPlus  != -1 );
    if( check1 && check2 && check3 ) {  hasPlusEqualSign = true;  }



    // STEP 2 Get operands
    TString tmp;
    Int_t   hOutIdx    =  0;
    Int_t   hIdxRange1 =  0;
    Int_t   hIdxRange2 =  0;

    Bool_t  IsValidEpr = false;
    if( hasPlusEqualSign &&  (idxAsterisk != -1))
    {

        // for: 99 += *
        tmp = epr(0,idxEqual);
        hOutIdx = tmp.Atoi();
        hOutIdx--; // since in the panel, we label histo[0] as spec no.1
        if( hOutIdx >=0 ) IsValidEpr = true;

    }

    Bool_t  IsValidEpr2 = false;
    if( hasPlusEqualSign && (s2 != "") ) {

        // for: 99 += 1..3
        tmp = epr(0,idxEqual);
        hOutIdx = tmp.Atoi();
        hOutIdx--; // since in the panel, we label histo[0] as spec no.1

        tmp = epr(idxEqual+1,idx1stDot);
        hIdxRange1 = tmp.Atoi();
        hIdxRange1--;

        tmp = epr(idx2ndDot+1, epr.Length() );
        hIdxRange2 = tmp.Atoi();
        hIdxRange2--;

        check1 = ( hOutIdx >=0 );
        check2 = ( hIdxRange1>=0 ) && ( hIdxRange1< fHistos->size() );
        check3 = ( hIdxRange2>=hIdxRange1 ) && ( hIdxRange2< fHistos->size() );
        if( check1 && check2 && check3 ) IsValidEpr2 = true;


    }





    Bool_t Is_operation_ok;
    TH1*    tempHisto;
    if( IsValidEpr ){

        // then we add up all the spectra
        // note: clone is not a good idea
        Int_t nbinsx  = fHistos->at(0)-> GetXaxis()-> GetNbins();
        Double_t xlow = fHistos->at(0)-> GetXaxis()-> GetXmin();
        Double_t xup  = fHistos->at(0)-> GetXaxis()-> GetXmax();
        TString tempTitle = Form("tempHisto%f", gRandom->Uniform() );
        tempHisto = new TH1F( tempTitle.Data(), "tempHisto", nbinsx, xlow, xup );



        for( Int_t i=0; i<fHistos->size(); i++ )
        {
            Is_operation_ok = tempHisto->Add( fHistos->at(i) );

            if( !Is_operation_ok ) break;
        }
    }

    if( Is_operation_ok && IsValidEpr ) {

        tempHisto->SetTitle("Adding Up all spectra");
        Bool_t createNewHisto = false;
        if( hOutIdx > 0 && hOutIdx >= fHistos->size() ) createNewHisto = true;


        if( createNewHisto )
        {
            fHistos->push_back( tempHisto  );
            *fHisto_list = Form( "%d", static_cast<int>(fHistos->size()-1) ) ;
            *fMessage =
            Form( "\nCreate a new spectrum at %d\n", static_cast<int>(fHistos->size())  );
        }
        else
        {
            fHistos->at(hOutIdx) = tempHisto;
            *fHisto_list = Form( "%d", hOutIdx) ;
            *fMessage = Form( "\nupdate the spectrum at %d\n", hOutIdx+1  );
        }

    }
    else {
         *fMessage = Form( "\nInvalid operation.\n" );
    }



    if( IsValidEpr2 ){

        // then we add up spectra from the selected range.
        // note: clone is not a good idea
        Int_t nbinsx  = fHistos->at(0)-> GetXaxis()-> GetNbins();
        Double_t xlow = fHistos->at(0)-> GetXaxis()-> GetXmin();
        Double_t xup  = fHistos->at(0)-> GetXaxis()-> GetXmax();
        TString tempTitle = Form("tempHisto%f", gRandom->Uniform() );
        tempHisto = new TH1F( tempTitle.Data(), "tempHisto", nbinsx, xlow, xup );



        for( Int_t i=hIdxRange1; i<=hIdxRange2; i++ )
        {
            Is_operation_ok = tempHisto->Add( fHistos->at(i) );

            if( !Is_operation_ok ) break;
        }
    }


    if( Is_operation_ok && IsValidEpr2 ) {

        tempHisto->
        SetTitle(Form("Adding Up from spec %d to %d", hIdxRange1+1,hIdxRange2+1 ) );
        Bool_t createNewHisto = false;
        if( hOutIdx > 0 && hOutIdx >= fHistos->size() ) createNewHisto = true;


        if( createNewHisto )
        {
            fHistos->push_back( tempHisto  );
            *fHisto_list = Form( "%d", static_cast<int>(fHistos->size()-1) ) ;
            *fMessage =
            Form( "\nCreate a new spectrum at %d\n", static_cast<int>(fHistos->size())  );
        }
        else
        {
            fHistos->at(hOutIdx) = tempHisto;
            *fHisto_list = Form( "%d", hOutIdx) ;
            *fMessage = Form( "\nupdate the spectrum at %d\n", hOutIdx+1  );
        }

    }
    else {
         *fMessage = Form( "\nInvalid operation.\n" );
    }




}


bool Dlg_Operation_histos::Special_case( TString epr ){

    Bool_t  ISsingleEqualSign = false;
    Bool_t  IsPlusEqualCombin = false;
    Bool_t  IsNoOtherOperator = true;
    TObjArray* objarray;
    Int_t substringN;

    // STEP 1: To check that we just have one "=", and no other operators.
    objarray = epr.Tokenize( "=" );
    substringN = objarray->GetEntries();
    if( substringN ==2 ) { ISsingleEqualSign=true; }

    objarray = epr.Tokenize( "+" );
    substringN = objarray->GetEntries();
    if( substringN >1 ) { IsNoOtherOperator=false; }

    objarray = epr.Tokenize( "-" );
    substringN = objarray->GetEntries();
    if( substringN >1 ) { IsNoOtherOperator=false; }

    objarray = epr.Tokenize( "*" );
    substringN = objarray->GetEntries();
    if( substringN >1 ) { IsNoOtherOperator=false; }

    objarray = epr.Tokenize( "/" );
    substringN = objarray->GetEntries();
    if( substringN >1 ) { IsNoOtherOperator=false; }






    Bool_t IsSpecialCase = false;

    if( ISsingleEqualSign && IsNoOtherOperator ) { IsSpecialCase = true; }



    // STEP 2 Get operands

    Bool_t IsValidEpr = true;
    //      0 1 2 3 4
    // ex.  3 = 2  ( histo[2] = histo[1] )
    Int_t   hOutIdx =  0;
    Int_t   hAIdx   =  0;
    TString tmp;

    if( IsSpecialCase ){


        Int_t    idxEqual= epr.Index("=");

        tmp = epr(0,idxEqual);
        hOutIdx = tmp.Atoi();
        hOutIdx--; // since in the panel, we label histo[0] as spec no.1

        tmp = epr( idxEqual+1, epr.Length() );
        hAIdx = tmp.Atoi();
        hAIdx--;

        // check hAIdx
        if( hAIdx >= fHistos->size() ||  hAIdx < 0 ) IsValidEpr = false;
        if( hOutIdx <0 ) IsValidEpr = false;

    }


    if( IsSpecialCase && IsValidEpr ) {



        Bool_t createNewHisto = false;
        if( hOutIdx > 0 && hOutIdx >= fHistos->size() ) createNewHisto = true;


        if( createNewHisto )
        {
            fHistos->push_back( (TH1*) fHistos->at(hAIdx)->Clone() );
            *fHisto_list = Form( "%d", static_cast<int>(fHistos->size()-1) ) ;
            *fMessage = Form( "\nCreate a new spectrum at %d\n", static_cast<int>(fHistos->size())  );
        }
        else
        {
            fHistos->at(hOutIdx) = (TH1*) fHistos->at(hAIdx)->Clone();
            *fHisto_list = Form( "%d", hOutIdx) ;
            *fMessage = Form( "\nupdate the spectrum at %d\n", hOutIdx+1  );
        }


        return 1;
    }
    else
    {
        *fMessage = Form( "\nInvalid operation.\n" );
    }

    return 0;
}


void Dlg_Operation_histos::General_case( TString epr ){

    Bool_t IsValidEpr   = false;
    Bool_t hasEqualSign = false;
    Bool_t hasPlusSign  = false;
    Bool_t hasMinusSign = false;
    Bool_t hasTimesSign = false;
    Int_t    idxEqual;
    Int_t    idxPlus;
    Int_t    idxMinus;
    Int_t    idxTimes;


    // STEP1 : validate the expression
    if( epr != "" )
    {
        //      0 1 2 3 4
        // ex.  2 = 2 + 1 ( histo[1] = histo[1] + histo[0]
        // ex  99 = 3 * 2 ( last one = 3 * histo[1] )

        // find out the index of =, + , -, *

        idxEqual = epr.Index("=");
        idxPlus  = epr.Index("+");
        idxMinus = epr.Index("-");
        idxTimes = epr.Index("*");

        if ( idxEqual != -1 )  hasEqualSign = true;
        if ( idxPlus  != -1 )  hasPlusSign  = true;
        if ( idxMinus != -1 )  hasMinusSign = true;
        if ( idxTimes != -1 )  hasTimesSign = true;

        if( hasEqualSign )
        {
            if( hasPlusSign || hasMinusSign || hasTimesSign ) { IsValidEpr = true; }
        }

    }



    // STEP2: Get operands
    Int_t hOutIdx =  0;
    Int_t hAIdx   =  0;
    Int_t hBIdx   =  0;
    Double_t mul  =  0;
    TString tmp;

    if( IsValidEpr )
    {
        //      0 1 2 3 4
        // ex.  2 = 2 + 1 ( histo[1] = histo[1] + histo[0]
        // ex  99 = 3 * 2 ( last one = 3 * histo[1] )

        tmp = epr(0,idxEqual); // slice it from index=0 to index=idxEqual
        hOutIdx = tmp.Atoi();
        hOutIdx--; // since in the panel, we label histo[0] as spec no.1




        if( hasPlusSign ) // + and - have two operands.
        {

            // hout = hA +  hB
            tmp = epr( idxEqual+1, idxPlus);
            hAIdx = tmp.Atoi();
            hAIdx--;

            tmp = epr( idxPlus+1, epr.Length() );
            hBIdx = tmp.Atoi();
            hBIdx--;
            if(0) printf("test hOutIdx = %d, hAIdx = %d, hBIdx = %d\n",
                        hOutIdx,  hAIdx, hBIdx );

        }
        else if ( hasMinusSign  && !hasTimesSign )
        {
            // hout = hA -  hB
            tmp = epr( idxEqual+1, idxMinus);
            hAIdx = tmp.Atoi();
            hAIdx--;

            tmp = epr( idxMinus+1, epr.Length() );
            hBIdx = tmp.Atoi();
            hBIdx--;
            if(0) printf("test hOutIdx = %d, hAIdx = %d, hBIdx = %d\n",hOutIdx,  hAIdx, hBIdx );


        }
        else if( hasTimesSign  )
        {
            // hout = mul *  hB
            tmp = epr( idxEqual+1, idxTimes);
            mul = tmp.Atof();


            tmp = epr( idxTimes+1, epr.Length() );
            hBIdx = tmp.Atoi();
            hBIdx--;
            if(0) printf("\ntest1 hOutIdx = %d, mul = %.1f, hBIdx = %d\n",hOutIdx,  mul, hBIdx );
        }



        // check hAIdx and hBIdx is within the range of histos.
        if( hAIdx > fHistos->size() ||  hAIdx < 0 ) IsValidEpr = false;
        if( hBIdx > fHistos->size() ||  hBIdx < 0 ) IsValidEpr = false;
        if( hOutIdx <0 ) IsValidEpr = false;
    }

    Bool_t createNewHisto = false;

    if( IsValidEpr ) {

        Bool_t Is_operation_ok = false;


        // note: clone is not a good idea
        Int_t nbinsx  = fHistos->at( hBIdx) -> GetXaxis()-> GetNbins();
        Double_t xlow = fHistos->at( hBIdx)-> GetXaxis()-> GetXmin();
        Double_t xup  = fHistos->at( hBIdx)-> GetXaxis()-> GetXmax();
        TH1* tempHisto = new TH1F( "tempHisto", "tempHisto", nbinsx, xlow, xup );


        if( hasPlusSign )
        {
            // hout = hA + hB
            Is_operation_ok
            = tempHisto->Add( fHistos->at(hAIdx)  ,  fHistos->at(hBIdx) );

            if( Is_operation_ok )
            {
                tempHisto -> SetTitle( Form("spec%02d + spec%02d", hAIdx+1, hBIdx+1 ));
                tempHisto -> SetName ( Form("spec%02d + spec%02d", hAIdx+1, hBIdx+1 ));
            }

        }
        else if ( hasMinusSign && !hasTimesSign )
        {
            // hout = hA - hB
            Is_operation_ok
            = tempHisto->Add( fHistos->at(hAIdx),  fHistos->at(hBIdx), 1 , -1 );

            if( Is_operation_ok )
            {
                tempHisto -> SetTitle( Form("spec%0d - spec%0d", hAIdx+1, hBIdx+1 ));
                tempHisto -> SetName ( Form("spec%0d - spec%0d", hAIdx+1, hBIdx+1 ));
            }
        }
        else if ( hasTimesSign )
        {
            // hout = mul *  hB
            Is_operation_ok
            = tempHisto->Add(  fHistos->at(hBIdx), mul );

            if(0) printf("\ntest2 hOutIdx = %d, mul = %.1f, hBIdx = %d\n",hOutIdx,  mul, hBIdx );


            if( Is_operation_ok )
            {
                tempHisto -> SetTitle( Form("spec%02d * (%.1f)", hBIdx+1, mul ) );
                tempHisto -> SetName ( Form("spec%02d * (%.1f)", hBIdx+1, mul ) );
            }
        }



        if( hOutIdx > 0 && hOutIdx >= fHistos->size() ) createNewHisto = true;


        if( Is_operation_ok )
        {
            if( createNewHisto )
            {
                fHistos->push_back( tempHisto );
                *fHisto_list = Form( "%d", static_cast<int>(fHistos->size()-1) ) ;
                *fMessage = Form( "\nCreate a new spectrum at %d\n", static_cast<int>(fHistos->size())  );
            }
            else
            {
                fHistos->at(hOutIdx) = tempHisto;
                *fHisto_list = Form( "%d", hOutIdx) ;
                *fMessage = Form( "\nupdate the spectrum at %d\n", hOutIdx+1  );
            }

        }

    }
    else
    {
         *fMessage = Form( "\nInvalid operation.\n" );
    }


}


void Dlg_Operation_histos::to_display_histos( TString epr){

    // when inputs are all digits, ex "1 3 5" or " 2 2 2"

    *fMessage = "open TH1 spectrum: ";

    // validate ( to avoid out the index range )
    TObjArray* tmp_array = epr.Tokenize(" ");
    Int_t substringN = tmp_array->GetEntries();

    for( Int_t i = 0; i < substringN; i++ ) {
        TString s_tmp = ( (TObjString*)tmp_array->At(i) )->GetString();
        int histo_idx = s_tmp.Atoi()-1;

        if ( histo_idx >=0 && histo_idx <= (fHistos->size()-1) ) {

            *fMessage += s_tmp + " ";
        }
    }

}

// this function parse the mathematical exprssion for the histogram
// operations. We will validate the input before apply the expression.
void Dlg_Operation_histos::Parse_expression( ){

    TString epr = fEntry_out; // epr = math expression

    if( !Special_case( epr ) ) { General_case( epr ); }

    // specail case for example: 2 = 1 ( just one equal sign )
    //                           7 += *
    //general case for example: 3 = 2 + 1, 99 = -1 * 2 ( one equal sign + one operator )


    if( epr.IsDigit() ) { to_display_histos( epr ); }

    else if( epr.Contains("+=") ) { combo_case( epr ); }

    else if( epr.Contains("del") ) { del_spectra( epr ); }

    else if( epr.Contains("let") ) { to_apply_formula( epr ); }

    CloseWindow();

}




// class constructor
Dlg_Operation_histos::Dlg_Operation_histos(
                const TGWindow* p,
                UInt_t w,
                UInt_t h,
                TString* histo_list,
                vector<TH1*>*  histos,
                TH1*           histo,
                TString* message )
: TGMainFrame(p, w, h) {

    SetCleanup(kDeepCleanup); // important step for closing windows properly.

    fHisto_list = histo_list;
    fHisto_list_backup= histo_list->Copy();

    fHisto  = histo;
    fHistos = histos;
    fMessage = message;

    TGLayoutHints*  Layout1 = new TGLayoutHints( kLHintsCenterY, 2, 2, 2, 2);
    TGLayoutHints*  Layout2 = new TGLayoutHints( kLHintsExpandX, 2, 2, 2, 2);
    TGLayoutHints*  Layout3 = new TGLayoutHints( kLHintsLeft, 2, 2, 2, 2);


    fHisto_frame =new TGVerticalFrame( this, 10, 10);
    Int_t nRow = 10;
    Int_t nCol = 0;
    fHisto_frame->SetLayoutManager ( new TGMatrixLayout( fHisto_frame , nRow, nCol) );

    fHisto_frame2 =new TGVerticalFrame( this, 10, 10);
    fHisto_frame2->SetLayoutManager ( new TGMatrixLayout( fHisto_frame2, nRow, nCol) );

    //-------------------------------------------------------| Labels

    TString outstr;
    TString histo_title;


    // to set 10 entries for each column.
    // for 101 to 200 entry we will put them into the secon layer.
    int n0 = histos->size() ;
    int n1 = histos->size() / 10;
    int n2 = ( n1 + 1) *10;
    if (n2 <= 10 ) n2 =20;



    if( n0 <= 100 ) {

        for (int i=0; i<n2 ; i++)
        {

            if( i  <histos->size() ) {
                histo_title = fHistos->at(i)->GetTitle();

                // to avoid long title
                if( n0 >= 30 ) {
                    if( histo_title.Length()>15 ) {
                        histo_title
                        = "..." + histo_title( histo_title.Length()-12, 12 ); }}

                outstr = Form( " %2d:  %s  ", i+1 , histo_title.Data() ) ;
            }
            else {
                histo_title = "";
                outstr = Form( "" ) ;
            }


            fHisto_nameLabel[i] = new TGLabel( fHisto_frame, outstr );
            fHisto_frame->AddFrame ( fHisto_nameLabel[i], Layout3 );

        }

        AddFrame( fHisto_frame, Layout1 );

    }
    else {


        for (int i=0; i<100 ; i++) {

            if( i  <histos->size() ) {
                histo_title = fHistos->at(i)->GetTitle();
                if( histo_title.Length()>15 ) {
                    histo_title
                    = "..." + histo_title( histo_title.Length()-12, 12 ); }
                outstr = Form( " %2d:  %s  ", i+1 , histo_title.Data() ) ;
            }
            else {
                histo_title = "";
                outstr = Form( "" ) ;
            }


            fHisto_nameLabel[i] = new TGLabel( fHisto_frame, outstr );
            fHisto_frame->AddFrame ( fHisto_nameLabel[i], Layout3 );

        }
        AddFrame( fHisto_frame, Layout1 );

        Int_t n3 = TMath::Min( 200, n0 );



        for (int i=100; i<n0 ; i++)
        {

            if( i < histos->size() ) {
                histo_title = fHistos->at(i)->GetTitle();
                if( histo_title.Length()>15 ) {
                    histo_title
                    = "..." + histo_title( histo_title.Length()-12, 12 ); }
                outstr = Form( " %3d:  %s  ", i+1 , histo_title.Data() ) ;
            }
            else {
                histo_title = "";
                outstr = Form( "" ) ;
            }


            fHisto_nameLabel[i] = new TGLabel( fHisto_frame2, outstr );
            fHisto_frame2->AddFrame ( fHisto_nameLabel[i], Layout3 );

        }

        AddFrame( fHisto_frame2, Layout1 );
    }

    //-------------------------------------------------------|






    fHF_1 = new TGHorizontalFrame( this , 1, 1);
    fHF_2 = new TGHorizontalFrame( this , 200, 30);


    //-------------------------------------------------------------------| text entry
    fTextEntries =
    new TGTextEntry(  fHF_1, fHisto_list->Data(), 1);    /* base, init value, ID */


    int width = fHisto_frame->GetDefaultWidth() ;
    fTextEntries->Resize( width, fTextEntries->GetDefaultHeight() );
    fHF_1->AddFrame( fTextEntries, Layout2 );



    //------------------------------------------------------------------| text entry: Connect

    fTextEntries
        -> Connect( "TextChanged(char*)",
                    "Dlg_Operation_histos", this, "Update_entry_output(char*)");

    fTextEntries
        -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_Operation_histos", this,
                    "To_response_key(Event_t*)");


    //-------------------------------------------------------------------| btn: close the window

    fBtn_close = new TGTextButton( fHF_2, "  OK  " );
    fBtn_close -> Connect( "Clicked()", "Dlg_Operation_histos", this, "CloseWindow()"  );
    fHF_2->AddFrame( fBtn_close, Layout2 );
    fBtn_close->Resize( 150, fBtn_close->GetDefaultHeight() );


    //-------------------------------------------------------------------| btn:  cancel the change
    fBtn_cancel = new TGTextButton( fHF_2, "Cancel" );
    fBtn_cancel -> Connect( "Clicked()", "Dlg_Operation_histos", this, "Cancel()"  );
    fHF_2->AddFrame( fBtn_cancel, Layout2 );
    fBtn_cancel->Resize(150, fBtn_cancel->GetDefaultHeight() );

    AddFrame( fHF_1, Layout1 );
    AddFrame( fHF_2, Layout2 );



    //-----------------------------------------------------------to focus on entry
    this->Connect(  "ProcessedEvent(Event_t*)", "Dlg_Operation_histos", this,
                    "To_focus_on_entry(Event_t*)" );




    SetName("Dlg_Operation_histos");
    SetWindowName("Histogram Operations");
    MapSubwindows();
    Resize( GetDefaultSize() );
    MapWindow();

    gClient->WaitFor(this);


}



