
//
//   Author : Pei-Luan Tai
//   Contact: pt10f@my.fsu.edu
//   Last update: Dec 3 2017
//***************************************/
#include <vector>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TObjArray.h>
#include <TString.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGTextEntry.h>
#include <TGWindow.h>
#include <TKey.h>
#include <TVirtualX.h>
#include <KeySymbols.h>
#include <TObjString.h>



class Dlg_Operation_histo2ds: public TGMainFrame {

private:

    vector<TH2*>*   fHisto2ds;
    TString*        fHisto2d_operated;
    TString         fHisto2d_operated_backup;
    TString*        fMessage;
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
    Dlg_Operation_histo2ds(
        const TGWindow* ,       /* base windows */
        UInt_t ,                /* width */
        UInt_t ,                /* height */
        TString* histo2d_list,   /* value passing by reference */
        vector<TH2*>*  histo2ds,
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

    void    to_display_a_TH2( TString epr );

    void    to_make_TCutG( TString epr);

    void    to_overlap_two_TH2( TString epr);

};


void Dlg_Operation_histo2ds::To_response_key(Event_t* e) {

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




// this function parse the mathematical exprssion for the histogram
// operations. We will validate the input before apply the expression.
void Dlg_Operation_histo2ds::Parse_expression( ){

    TString epr = fEntry_out; // epr = expression


    // define the variables for process
    Bool_t  go_special_case = false; //  99 = 1 (just one equal sign)
    Bool_t  go_general_case = false; //  99 = 3 * 2 ( one equal and operator sign) 

    Bool_t  hasSingleEqualSign  = false;
    Bool_t  hasEqualSign  = false;
    Bool_t  hasPlusSign   = false;
    Bool_t  hasMinusSign  = false;
    Bool_t  hasTimesSign  = false;

    Bool_t  IsNoOtherOperator = true;
    TObjArray* objarray;
    Int_t substringN;

    // to check single equal sign
    objarray = epr.Tokenize( "=" );
    substringN = objarray->GetEntries();
    if( substringN ==2 ) { hasSingleEqualSign=true; }

    // to check "=, +, -, * " sign
    Int_t idxEqual = epr.Index("=");
    Int_t idxPlus  = epr.Index("+");
    Int_t idxMinus = epr.Index("-");
    Int_t idxTimes = epr.Index("*");
    if ( idxEqual != -1 )  hasEqualSign = true;
    if ( idxPlus  != -1 )  hasPlusSign  = true;
    if ( idxMinus != -1 )  hasMinusSign = true;
    if ( idxTimes != -1 )  hasTimesSign = true;

    if( hasSingleEqualSign && !hasPlusSign && !hasMinusSign && !hasTimesSign ) {
        //  ( just one equal sign )
        go_special_case = true;
    }

    if( hasSingleEqualSign && ( hasPlusSign || hasMinusSign || hasTimesSign) ){
        // ( one equal sign + one operator )
        if( !epr.Contains("+=") ) go_general_case = true;
    }


    if( epr.IsDigit() ) { to_display_a_TH2(epr); }
    
    else if( epr.Contains("cut") ) { to_make_TCutG(epr); }

    else if( epr.Contains("overlap") ) { to_overlap_two_TH2(epr); }

    else if( go_special_case ) { Special_case( epr );  }

    else if( go_general_case ) { General_case( epr );  }

    else if( epr.SubString("+=") != "" ) {  combo_case( epr ); }

    else if( epr.SubString("del") != "" ) { del_spectra( epr); }

    CloseWindow();

}





void Dlg_Operation_histo2ds::Cancel(){

    // cancel the change, and use the original setting.
    *fHisto2d_operated = fHisto2d_operated_backup.Data();
    CloseWindow();
 }


void Dlg_Operation_histo2ds::CloseWindow(){

    fBtn_close->SetState(kButtonDisabled); // to avoid double click.
    fBtn_cancel->SetState(kButtonDisabled);

    DeleteWindow();
 }



void Dlg_Operation_histo2ds::To_focus_on_entry( Event_t* e) {

    fTextEntries->SetFocus();
    fTextEntries->SelectAll();
}




void Dlg_Operation_histo2ds::Update_entry_output( char* entry_output ) {

    fEntry_out = entry_output;
}


void Dlg_Operation_histo2ds::del_spectra( TString epr ){
    
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
    if( epr.Contains("..") ) {

        tmp = epr( idxDel,idx1stDot);
        hIdxRange1 = tmp.Atoi();
        hIdxRange1--; // since in the panel, we label histo[0] as spec no.1

        tmp = epr(idx2ndDot+1, epr.Length() );
        hIdxRange2 = tmp.Atoi();
        hIdxRange2--;

        check1 = ( hIdxRange1>=0 ) && ( hIdxRange1< fHisto2ds->size() );
        check2 = ( hIdxRange2>=hIdxRange1 ) && ( hIdxRange2< fHisto2ds->size() );

        if( check1 && check2 ) IsValidEpr = true;
    }

    if( IsValidEpr ) {

        // delete spectra
        Int_t del_operation_N = hIdxRange2 - hIdxRange1 + 1;
        Int_t starting_idx = hIdxRange1;
        for( Int_t i=0; i<del_operation_N; i++ ) {

            // make sure we have at least one histogram.
            if( fHisto2ds->size() >= 2 ){

                fHisto2ds->erase( fHisto2ds->begin() + starting_idx );

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

            if( idx < fHisto2ds->size() ) {

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

            fHisto2ds->erase( fHisto2ds->begin() + starting_idx );

        }

         *fMessage = Form("delte spec %s", msg.Data() ) ;
    }
}


void Dlg_Operation_histo2ds::combo_case( TString epr ) {

    // this block is to deal with "99 += 1..32" and " 99 += * "
    // we have checked "+=" sign, before go in to this method.
    
    Int_t    idxEqual= epr.Index("=");
    Int_t    idxPlus = epr.Index("+");
    Int_t    idxAsterisk = epr.Index("*");
    Int_t    idx1stDot = -1;
    Int_t    idx2ndDot = -1;
    TString  s2 = epr.SubString( ".." );
    if( s2 != "" ) { idx1stDot = epr.Index(".");  idx2ndDot = idx1stDot + 1; }



    // STEP 1 Get operands
    TString tmp;
    Int_t   hOutIdx    =  0;
    Int_t   hIdxRange1 =  0;
    Int_t   hIdxRange2 =  0;

    Bool_t  IsValidEpr = false;
    Bool_t  IsValidEpr2 = false;
    if( !epr.Contains("..") )
    {
        // for: 99 += *
        tmp = epr(0,idxEqual);
        hOutIdx = tmp.Atoi();
        hOutIdx--; // since in the panel, we label histo[0] as spec no.1
        if( hOutIdx >=0 ) IsValidEpr = true;

    }
    else {

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

        bool check1 = ( hOutIdx >=0 );
        bool check2 = ( hIdxRange1>=0 ) && ( hIdxRange1< fHisto2ds->size() );
        bool check3 = ( hIdxRange2>=hIdxRange1 ) && ( hIdxRange2< fHisto2ds->size() );
        if( check1 && check2 && check3 ) IsValidEpr2 = true;

    }





    Bool_t Is_operation_ok;
    TH2*    tempHisto;
    if( IsValidEpr ){

        // then we add up all the spectra
        // note: clone is not a good idea
        Int_t nbinsx  = fHisto2ds->at(0)-> GetXaxis()-> GetNbins();
        Double_t xlow = fHisto2ds->at(0)-> GetXaxis()-> GetXmin();
        Double_t xup  = fHisto2ds->at(0)-> GetXaxis()-> GetXmax();
        Int_t nbinsy  = fHisto2ds->at(0)-> GetYaxis()-> GetNbins();
        Double_t ylow = fHisto2ds->at(0)-> GetYaxis()-> GetXmin();
        Double_t yup  = fHisto2ds->at(0)-> GetYaxis()-> GetXmax();
        TString tempTitle = Form("tempHisto%f", gRandom->Uniform() );
        tempHisto = new TH2F( tempTitle.Data(), "tempHisto", 
                                    nbinsx, xlow, xup, 
                                    nbinsy, ylow, yup );


        for( Int_t i=0; i<fHisto2ds->size(); i++ )
        {
            Is_operation_ok = tempHisto->Add( fHisto2ds->at(i) );

            if( !Is_operation_ok ) break;
        }
    }

    if( Is_operation_ok && IsValidEpr ) {

        tempHisto->SetTitle("Adding Up all spectra");
        Bool_t createNewHisto = false;
        if( hOutIdx > 0 && hOutIdx >= fHisto2ds->size() ) createNewHisto = true;


        if( createNewHisto )
        {
            fHisto2ds->push_back( tempHisto  );
            *fHisto2d_operated = Form( "%d", static_cast<int>(fHisto2ds->size()-1) ) ;
            *fMessage =
            Form( "\nCreate a new spectrum at %d\n", static_cast<int>(fHisto2ds->size())  );
        }
        else
        {
            fHisto2ds->at(hOutIdx) = tempHisto;
            *fHisto2d_operated = Form( "%d", hOutIdx) ;
            *fMessage = Form( "\nupdate the spectrum at %d\n", hOutIdx+1  );
        }

    }
    else {
         *fMessage = Form( "\nInvalid operation.\n" );
    }



    if( IsValidEpr2 ){

        // then we add up spectra from the selected range.
        // note: clone is not a good idea
        Int_t nbinsx  = fHisto2ds->at(0)-> GetXaxis()-> GetNbins();
        Double_t xlow = fHisto2ds->at(0)-> GetXaxis()-> GetXmin();
        Double_t xup  = fHisto2ds->at(0)-> GetXaxis()-> GetXmax();
        Int_t nbinsy  = fHisto2ds->at(0)-> GetYaxis()-> GetNbins();
        Double_t ylow = fHisto2ds->at(0)-> GetYaxis()-> GetXmin();
        Double_t yup  = fHisto2ds->at(0)-> GetYaxis()-> GetXmax();
        TString tempTitle = Form("tempHisto%f", gRandom->Uniform() );
        tempHisto = new TH2F( tempTitle.Data(), "tempHisto",
                                    nbinsx, xlow, xup, 
                                    nbinsy, ylow, yup );


        for( Int_t i=hIdxRange1; i<=hIdxRange2; i++ )
        {
            Is_operation_ok = tempHisto->Add( fHisto2ds->at(i) );

            if( !Is_operation_ok ) break;
        }
    }


    if( Is_operation_ok && IsValidEpr2 ) {

        tempHisto->
        SetTitle(Form("Adding Up from spec %d to %d", hIdxRange1+1,hIdxRange2+1 ) );
        Bool_t createNewHisto = false;
        if( hOutIdx > 0 && hOutIdx >= fHisto2ds->size() ) createNewHisto = true;


        if( createNewHisto )
        {
            fHisto2ds->push_back( tempHisto  );
            *fHisto2d_operated = Form( "%d", static_cast<int>(fHisto2ds->size()-1) ) ;
            *fMessage =
            Form( "\nCreate a new spectrum at %d\n", static_cast<int>(fHisto2ds->size())  );
        }
        else
        {
            fHisto2ds->at(hOutIdx) = tempHisto;
            *fHisto2d_operated = Form( "%d", hOutIdx) ;
            *fMessage = Form( "\nupdate the spectrum at %d\n", hOutIdx+1  );
        }

    }
    else {
         *fMessage = Form( "\nInvalid operation.\n" );
    }




}


void Dlg_Operation_histo2ds::to_display_a_TH2( TString epr) {

    *fMessage = "to show TH2: ";

    TObjArray* tmp_array = epr.Tokenize(" ");
    Int_t substringN = tmp_array->GetEntries();

    // we will add the valid histo idx to the fHisto2d_operated ( TString )
    for( Int_t i = 0; i < substringN; i++ ) {
        TString s_tmp = ( (TObjString*)tmp_array->At(i) )->GetString();
        int histo_idx = s_tmp.Atoi() - 1;  
 	
	// validate ( to avoid out the index range )
	if( histo_idx >= 0 && histo_idx<fHisto2ds->size() ) {
	    *fHisto2d_operated += Form( "%d ", histo_idx );
	}
    }

}



void Dlg_Operation_histo2ds::to_overlap_two_TH2( TString epr) {


    TObjArray* tmp_array = epr.Tokenize(" ");
    if( tmp_array->GetEntries() >= 3 ) {
        // 
        // to deal with input as "overlap 1 2"
        //
        *fHisto2d_operated = ( (TObjString*)tmp_array->At(1) )->GetString();

        *fHisto2d_operated += " ";

        *fHisto2d_operated += ( (TObjString*)tmp_array->At(2) )->GetString();

	*fMessage = "to overlap TH2: ";
    }
}

void Dlg_Operation_histo2ds::to_make_TCutG( TString epr) { 

    *fMessage = "to cut TH2: ";

    TObjArray* tmp_array = epr.Tokenize(" ");
    if( tmp_array->GetEntries() >= 2 ) {
        // 
        // to deal with input as "cut 2"
        // it will diplay spectrum no.2 and then make a TCutG
        //
        *fHisto2d_operated = ( (TObjString*)tmp_array->At(1) )->GetString();


        if( fHisto2d_operated->IsDigit() ) {
            *fMessage += *fHisto2d_operated;
        } else {
            *fHisto2d_operated = "1"; 
        }
    }
    else if( tmp_array->GetEntries() == 1 ) {

        //
        // to deal with input as "cut" without TH2 object number.
        // it will make a TCutG at the current TH2 object.
        //
        *fHisto2d_operated = "-99";

    }

}

bool Dlg_Operation_histo2ds::Special_case( TString epr ){

    // we just have one "=", and no other operators.
    TObjArray* objarray;
    Int_t substringN;





    // STEP 1 Get operands

    Bool_t IsValidEpr = true;
    //      0 1 2 3 4
    // ex.  3 = 2  ( histo[2] = histo[1] )
    Int_t   hOutIdx =  0;
    Int_t   hAIdx   =  0;
    TString tmp;


    Int_t    idxEqual= epr.Index("=");
    tmp = epr(0,idxEqual);
    hOutIdx = tmp.Atoi();
    hOutIdx--; // since in the panel, we label histo[0] as spec no.1

    tmp = epr( idxEqual+1, epr.Length() );
    hAIdx = tmp.Atoi();
    hAIdx--;

    // check hAIdx
    if( hAIdx >= fHisto2ds->size() ||  hAIdx < 0 ) IsValidEpr = false;
    if( hOutIdx <0 ) IsValidEpr = false;



    if( IsValidEpr ) {

        Bool_t createNewHisto = false;
        if( hOutIdx > 0 && hOutIdx >= fHisto2ds->size() ) createNewHisto = true;


        if( createNewHisto )
        {
            fHisto2ds->push_back( (TH2*) fHisto2ds->at(hAIdx)->Clone() );
            *fHisto2d_operated = Form( "%d", static_cast<int>(fHisto2ds->size()-1) ) ;
            *fMessage = Form( "\nCreate a new spectrum at %d\n", 
                                static_cast<int>(fHisto2ds->size())  );
        }
        else
        {
            fHisto2ds->at(hOutIdx) = (TH2*) fHisto2ds->at(hAIdx)->Clone();
            *fHisto2d_operated = Form( "%d", hOutIdx) ;
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


void Dlg_Operation_histo2ds::General_case( TString epr ){

    //      0 1 2 3 4
    // ex.  2 = 2 + 1 ( histo[1] = histo[1] + histo[0]
    // ex  99 = 3 * 2 ( last one = 3 * histo[1] )
    
    
    // STEP1: Get operands
    Int_t hOutIdx =  0;
    Int_t hAIdx   =  0;
    Int_t hBIdx   =  0;
    Double_t mul  =  0;
    TString tmp;

    Bool_t  hasEqualSign  = false;
    Bool_t  hasPlusSign   = false;
    Bool_t  hasMinusSign  = false;
    Bool_t  hasTimesSign  = false;

    Int_t idxEqual = epr.Index("=");
    Int_t idxPlus  = epr.Index("+");
    Int_t idxMinus = epr.Index("-");
    Int_t idxTimes = epr.Index("*");
    if ( idxEqual != -1 )  hasEqualSign = true;
    if ( idxPlus  != -1 )  hasPlusSign  = true;
    if ( idxMinus != -1 )  hasMinusSign = true;
    if ( idxTimes != -1 )  hasTimesSign = true;
    
     tmp = epr(0,idxEqual); // slice it from index=0 to index=idxEqual
    hOutIdx = tmp.Atoi();
    hOutIdx--; // since in the panel, we label histo[0] as spec no.1


    if( hasPlusSign ) {
        // hout = hA + hB
        tmp = epr( idxEqual+1, idxPlus);
        hAIdx = tmp.Atoi();
        hAIdx--;

        tmp = epr( idxPlus+1, epr.Length() );
        hBIdx = tmp.Atoi();
        hBIdx--;
    }
    else if ( hasMinusSign  && !hasTimesSign ) {
        // hout = hA - hB
        tmp = epr( idxEqual+1, idxMinus);
        hAIdx = tmp.Atoi();
        hAIdx--;

        tmp = epr( idxMinus+1, epr.Length() );
        hBIdx = tmp.Atoi();
        hBIdx--;
    }
    else if( hasTimesSign  ) {
        // hout = mul *  hB
        tmp = epr( idxEqual+1, idxTimes);
        mul = tmp.Atof();

        tmp = epr( idxTimes+1, epr.Length() );
        hBIdx = tmp.Atoi();
        hBIdx--;
    }

    bool createNewHisto = false;
    bool IsValidEpr = true; 
    // check hAIdx and hBIdx is within the range of histo2ds.
    if( hAIdx > fHisto2ds->size() ||  hAIdx < 0 ) IsValidEpr = false;
    if( hBIdx > fHisto2ds->size() ||  hBIdx < 0 ) IsValidEpr = false;
    if( hOutIdx <0 ) IsValidEpr = false;

    if( IsValidEpr ) {

        Bool_t Is_operation_ok = false;

        // note: clone is not a good idea
        Int_t nbinsx  = fHisto2ds->at( hBIdx)-> GetXaxis()-> GetNbins();
        Double_t xlow = fHisto2ds->at( hBIdx)-> GetXaxis()-> GetXmin();
        Double_t xup  = fHisto2ds->at( hBIdx)-> GetXaxis()-> GetXmax();
        Int_t nbinsy  = fHisto2ds->at( hBIdx)-> GetYaxis()-> GetNbins();
        Double_t ylow = fHisto2ds->at( hBIdx)-> GetYaxis()-> GetXmin();
        Double_t yup  = fHisto2ds->at( hBIdx)-> GetYaxis()-> GetXmax();
        TH2* tempHisto = new TH2F( "tempHisto", "tempHisto", 
                                    nbinsx, xlow, xup, 
                                    nbinsy, ylow, yup );


        if( hasPlusSign )
        {
            // hout = hA + hB
            Is_operation_ok
            = tempHisto->Add( fHisto2ds->at(hAIdx)  ,  fHisto2ds->at(hBIdx) );
	    
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
            = tempHisto->Add( fHisto2ds->at(hAIdx),  fHisto2ds->at(hBIdx), 1 , -1 );

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
            = tempHisto->Add(  fHisto2ds->at(hBIdx), mul );


            if( Is_operation_ok )
            {
                tempHisto -> SetTitle( Form("spec%02d * (%.1f)", hBIdx+1, mul ) );
                tempHisto -> SetName ( Form("spec%02d * (%.1f)", hBIdx+1, mul ) );
            }
        }



        if( hOutIdx > 0 && hOutIdx >= fHisto2ds->size() ) createNewHisto = true;
	

        if( Is_operation_ok )
        {
            if( createNewHisto )
            {
                fHisto2ds->push_back( tempHisto );
                *fHisto2d_operated = Form( "%d", static_cast<int>(fHisto2ds->size()-1) ) ;
                *fMessage = Form( "\nCreate a new spectrum at %d\n", 
                             static_cast<int>(fHisto2ds->size())  );
            }
            else
            {
                fHisto2ds->at(hOutIdx) = tempHisto;
                *fHisto2d_operated = Form( "%d", hOutIdx) ;
                *fMessage = Form( "\nupdate the spectrum at %d\n", hOutIdx+1  );
            }

        }

    }
    else
    {
         *fMessage = Form( "\nInvalid operation.\n" );
    }


}



// class constructor
Dlg_Operation_histo2ds::Dlg_Operation_histo2ds(
                const TGWindow* p,
                UInt_t w,
                UInt_t h,
                TString* histo2d_list,
                vector<TH2*>*  histo2ds,
                TString* message )
: TGMainFrame(p, w, h) {

    SetCleanup(kDeepCleanup); // important step for closing windows properly.


    fHisto2d_operated = histo2d_list;
    fHisto2d_operated_backup= histo2d_list->Copy();

    fHisto2ds = histo2ds;
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
    TString histo2d_title;


    // to set 10 entries for each column.
    // for 101 to 200 entry we will put them into the secon layer.
    int n0 = histo2ds->size() ;
    int n1 = histo2ds->size() / 10;
    int n2 = ( n1 + 1) *10;
    if (n2 <= 10 ) n2 =20;



    if( n0 <= 100 ) {

        for (int i=0; i<n2 ; i++)
        {

            if( i  < histo2ds->size() ) {
                histo2d_title = fHisto2ds->at(i)->GetTitle();
                if( histo2d_title.Length()>15 ) {
                    histo2d_title
                    = "..." + histo2d_title( histo2d_title.Length()-12, 12 ); }
                outstr = Form( " [2D] %2d:  %s  ", i+1 , histo2d_title.Data() ) ;
            }
            else {
                histo2d_title = "";
                outstr = Form( "" ) ;
            }


            fHisto_nameLabel[i] = new TGLabel( fHisto_frame, outstr );
            fHisto_frame->AddFrame ( fHisto_nameLabel[i], Layout3 );

        }

        AddFrame( fHisto_frame, Layout1 );

    }
    else {


        for (int i=0; i<100 ; i++) {

            if( i  <histo2ds->size() ) {
                histo2d_title = fHisto2ds->at(i)->GetTitle();
                if( histo2d_title.Length()>15 ) {
                    histo2d_title
                    = "..." + histo2d_title( histo2d_title.Length()-12, 12 ); }
                outstr = Form( " [2D] %2d:  %s  ", i+1 , histo2d_title.Data() ) ;
            }
            else {
                histo2d_title = "";
                outstr = Form( "" ) ;
            }


            fHisto_nameLabel[i] = new TGLabel( fHisto_frame, outstr );
            fHisto_frame->AddFrame ( fHisto_nameLabel[i], Layout3 );

        }
        AddFrame( fHisto_frame, Layout1 );



        for (int i=100; i<n0 ; i++)
        {

            if( i < histo2ds->size() ) {
                histo2d_title = fHisto2ds->at(i)->GetTitle();
                if( histo2d_title.Length()>15 ) {
                    histo2d_title
                    = "..." + histo2d_title( histo2d_title.Length()-12, 12 ); }
                outstr = Form( " %3d:  %s  ", i+1 , histo2d_title.Data() ) ;
            }
            else {
                histo2d_title = "";
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
    new TGTextEntry(  fHF_1, fHisto2d_operated->Data(), 1);    /* base, init value, ID */

    int width = fHisto_frame->GetDefaultWidth() ;
    fTextEntries->Resize( width, fTextEntries->GetDefaultHeight() );
    fHF_1->AddFrame( fTextEntries, Layout2 );



    //------------------------------------------------------------------| text entry: Connect

    fTextEntries
        -> Connect( "TextChanged(char*)",
                    "Dlg_Operation_histo2ds", this, "Update_entry_output(char*)");

    fTextEntries
        -> Connect( "ProcessedEvent(Event_t*)",
                    "Dlg_Operation_histo2ds", this,
                    "To_response_key(Event_t*)");


    //-------------------------------------------------------------------| btn: close the window

    fBtn_close = new TGTextButton( fHF_2, "  OK  " );
    fBtn_close -> Connect( "Clicked()", "Dlg_Operation_histo2ds", this, "CloseWindow()"  );
    fHF_2->AddFrame( fBtn_close, Layout2 );
    fBtn_close->Resize( 150, fBtn_close->GetDefaultHeight() );


    //-------------------------------------------------------------------| btn:  cancel the change
    fBtn_cancel = new TGTextButton( fHF_2, "Cancel" );
    fBtn_cancel -> Connect( "Clicked()", "Dlg_Operation_histo2ds", this, "Cancel()"  );
    fHF_2->AddFrame( fBtn_cancel, Layout2 );
    fBtn_cancel->Resize(150, fBtn_cancel->GetDefaultHeight() );

    AddFrame( fHF_1, Layout1 );
    AddFrame( fHF_2, Layout2 );



    //-----------------------------------------------------------to focus on entry
    this->Connect(  "ProcessedEvent(Event_t*)", "Dlg_Operation_histo2ds", this,
                    "To_focus_on_entry(Event_t*)" );




    SetName("Dlg_Operation_histo2ds");
    SetWindowName("2D Histogram Operations");
    MapSubwindows();
    Resize( GetDefaultSize() );
    MapWindow();

    gClient->WaitFor(this);


}

/////////////////////////////////////////////////////////////////////////////////////

