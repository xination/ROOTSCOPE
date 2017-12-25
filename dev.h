

/////////////////////////////////////////////////////////////////////////////////////



class Dlg_add_note: public TGTransientFrame {

    TString*  fnote;

    TGHorizontalFrame*  fHF;


    TGTextEntry*        fTextEntries;
    TGTextButton*       fBtn_close;
    TGTextButton*       fBtn_cancel;



public:

    // class constructor
    Dlg_add_note( const TGWindow* ,    /* base windows */
                    const TGWindow*,     /* the windows transient from */
                    UInt_t ,             /* width */
                    UInt_t ,             /* height */
                    TString* );

    void    To_response_key( Event_t* );

    void    To_focus_on_entry( Event_t* );

    void    CloseWindow();

    void    Cancel();

    void    Update_note(char*  );


};




void Dlg_add_note::Update_note(char* entry_output){


        *fnote = entry_output;
}


void Dlg_add_note::Cancel(){

    // cancel the change.
    *fnote = "";
    CloseWindow();
 }


void Dlg_add_note::CloseWindow(){

    fBtn_close->SetState(kButtonDisabled); // to avoid double click.
    fBtn_cancel->SetState(kButtonDisabled);

    DeleteWindow();
 }



void Dlg_add_note::To_focus_on_entry( Event_t* e) {

    fTextEntries->SetFocus();
    fTextEntries->SelectAll();
}


void Dlg_add_note::To_response_key(Event_t* e) {

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
Dlg_add_note::Dlg_add_note( const TGWindow* p,
                                const TGWindow* main,
                                UInt_t w,
                                UInt_t h,
                                TString* title)
: TGTransientFrame(p, main, w, h)
{

    SetCleanup(kDeepCleanup); // important step for closing windows properly.

    fnote = title;

    TGLayoutHints*  Layout1 = new TGLayoutHints( kLHintsCenterY, 2, 2, 2, 2);

    fHF = new TGHorizontalFrame( this, 200, 30 );


    //-------------------------------------------------------------------| text entry
    fTextEntries =
    new TGTextEntry( fHF, fnote->Data(), 1);    /* base, init value, ID */

    fTextEntries->Resize( 300, fTextEntries->GetDefaultHeight() );

    fHF->AddFrame( fTextEntries, Layout1 );

    //------------------------------------------------------------------| Connect
    fTextEntries
        -> Connect( "TextChanged(char*)",
                    "Dlg_add_note", this, "Update_note(char*)");

    fTextEntries
        -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_add_note", this,
                    "To_response_key(Event_t*)");

    //-------------------------------------------------------------------| btn: close the window

    fBtn_close = new TGTextButton( fHF, "  OK  " );
    fBtn_close -> Connect( "Clicked()", "Dlg_add_note", this, "CloseWindow()"  );
    fHF->AddFrame( fBtn_close, Layout1 );
    fBtn_close->Resize( 150, fBtn_close->GetDefaultHeight() );


    //-------------------------------------------------------------------| btn:  cancel the change
    fBtn_cancel = new TGTextButton( fHF, "Cancel" );
    fBtn_cancel -> Connect( "Clicked()", "Dlg_add_note", this, "Cancel()"  );
    fHF->AddFrame( fBtn_cancel, Layout1 );
    fBtn_cancel->Resize(150, fBtn_cancel->GetDefaultHeight() );

    AddFrame( fHF, Layout1 );


    //-----------------------------------------------------------to focus on entry
    this->Connect(  "ProcessedEvent(Event_t*)", "Dlg_add_note", this,
                    "To_focus_on_entry(Event_t*)" );



    SetName("Dlg_add_note");
    SetWindowName("Add note");
    MapSubwindows();
    Resize( GetDefaultSize() );
    MapWindow();
    gClient->WaitFor(this);
}

