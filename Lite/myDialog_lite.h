//
//   Author : Pei-Luan Tai
//   Contact: pt10f@my.fsu.edu
//   Last update: May 2, 2017
//***************************************/
#include <TH1.h>
#include <TH2.h>
#include <TGNumberEntry.h>
#include <TString.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TVirtualX.h>
#include <KeySymbols.h>
#include <TObjString.h>
#include <TObjArray.h>



////////////////////////////////////////////////////////////////////
// function for fitting.
double gauss_bg(  Double_t *t, Double_t *par)
{

    Double_t x = t[0];
    Double_t const_1   = par[0];
    Double_t linear_1  = par[1];
    Double_t height  = par[2];
    Double_t center  = par[3];
    Double_t sigma   = par[4];


    Double_t y;

    y = const_1 +  linear_1 * x +
        height * exp ( -0.5 * pow( (x-center)/sigma, 2 )  );

    return y;

}

double double_gauss_bg(  Double_t *t, Double_t *par)
{

    Double_t x = t[0];

    Double_t const_1   = par[0];
    Double_t linear_1  = par[1];
    Double_t height1   = par[2];
    Double_t center1   = par[3];
    Double_t sigma1    = par[4];
    Double_t height2   = par[5];
    Double_t center2   = par[6];
    Double_t sigma2    = par[7];

    // 8 parameters


    Double_t y;

    y = const_1 +  linear_1 * x +
        height1 * exp ( -0.5 * pow( (x-center1)/sigma1, 2 )  ) +
        height2 * exp ( -0.5 * pow( (x-center2)/sigma2, 2 )  )
        ;

    return y;

}




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

        if( key_symbol == kKey_Enter   ) {  Dlg_Set_Background::CloseWindow(); } // the enter key near num pad

        else if ( e->fCode == key_enter ) {  Dlg_Set_Background::CloseWindow(); } // the enter key in regular position.

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



    //-------------------------------------------------------------------| A Button for close the window
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

        if( key_symbol == kKey_Enter   ) {  Dlg_Set_Marker::CloseWindow(); } // the enter key near num pad

        else if ( e->fCode == key_enter ) {  Dlg_Set_Marker::CloseWindow(); } // the enter key in regular position.

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


    // current I just set one label text.
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



///////////////////////////////////////////////////////////////////////////////////////

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
                    TString* );          /* bg value passing by reference */

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


/////////////////////////////////////////////////////////////////




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

    bool    ToParseString ( TString s_input ,
                            const char* delim,
                            TString* substrings  );

};


// my function to parse a TString which is split by delim
// avoid to use vector for root 5
bool Dlg_Set_XRange::ToParseString
( TString s_input , const char* delim, TString* substrings  )
{

    TObjArray* objarray = s_input.Tokenize( delim );
    Int_t substringN = objarray->GetEntries();

    if ( substringN != 2 ) { return false; }
    else {
        TString tmp;
        tmp =  ( (TObjString*)objarray->At(0) ) -> GetString();
        substrings[0] = Form( "%s", tmp.Data() ) ;

        tmp =  ( (TObjString*)objarray->At(1) ) -> GetString();
        substrings[1] = Form( "%s", tmp.Data() ) ;

        return true;
    }


}


void Dlg_Set_XRange::Update_entry_output( char* entry_output ) {

    fEntry_out = entry_output;
}


// update the customized bg value
void Dlg_Set_XRange::Update_marker( ){

        if( fEntry_out != "" )
        {


            TString ranges[2];
            Bool_t  IsParseOK
            = ToParseString( fEntry_out, " ", ranges );

            if( IsParseOK ) {

                TString s1 = ranges[0] ;
                TString s2 = ranges[1] ;

                *fUserMax = TMath::Max(   s1.Atof(),   s2.Atof() );
                *fUserMin = TMath::Min(   s1.Atof(),   s2.Atof() );
                CloseWindow();
            }
            else {
                *fUserMax = 0;
                *fUserMin = 0;
            }
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

    //-------------------------------------------------------------------| Btn for close the window
    fBtn_close = new TGTextButton( fHF_btns, "OK" );
    fBtn_close -> Connect( "Clicked()", "Dlg_Set_XRange", this, "CloseWindow()"  );
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

    bool    ToParseString ( TString s_input ,
                            const char* delim,
                            TString* substrings  );

};


void Dlg_Change_histo_X_Range::Update_entry_output( char* entry_output ) {

    fEntry_out = entry_output;
}


// my function to parse a TString which is split by delim
// avoid to use vector for root 5
bool Dlg_Change_histo_X_Range::ToParseString
( TString s_input , const char* delim, TString* substrings  )
{

    TObjArray* objarray = s_input.Tokenize( delim );
    Int_t substringN = objarray->GetEntries();

    if ( substringN != 2 ) { return false; }
    else {
        TString tmp;
        tmp =  ( (TObjString*)objarray->At(0) ) -> GetString();
        substrings[0] = Form( "%s", tmp.Data() ) ;

        tmp =  ( (TObjString*)objarray->At(1) ) -> GetString();
        substrings[1] = Form( "%s", tmp.Data() ) ;

        return true;
    }


}



// update the customized bg value
void Dlg_Change_histo_X_Range::Update_marker( ){

        if( fEntry_out != "" )
        {

            TString ranges[2];
            Bool_t  IsParseOK
            = ToParseString( fEntry_out, " ", ranges );

            if( IsParseOK ) {

                TString s1 = ranges[0] ;
                TString s2 = ranges[1] ;

                *fXup_select = TMath::Max(   s1.Atof(),   s2.Atof() );
                *fXlow_select = TMath::Min(   s1.Atof(),   s2.Atof() );
                CloseWindow();

            }
            else {

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

//////////////////////////////////////////////////////////////////


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











////////////////////////////////////////////////////////////////////////


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
