/*
*   Author : Pei-Luan Tai
*   Contact: pt10f@my.fsu.edu
*   Last update: May 2, 2017
***************************************/
#include <vector>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TList.h>
#include <TKey.h>
#include <TString.h>


class ReadIn_and_parse {

private:

    vector<TH1*>*   fHistos;
    TH2*            fHisto2d;
    Bool_t          HasTH2;
    TFile*          fInFile;
    Int_t           fCntHistos;

    TH1C*   h1c;
    TH1S*   h1s;
    TH1I*   h1i;
    TH1F*   h1f;
    TH1D*   h1d;

    TH2C*   h2c;
    TH2S*   h2s;
    TH2I*   h2i;
    TH2F*   h2f;
    TH2D*   h2d;




public:



    // constructor
    ReadIn_and_parse( vector<TH1*>*  histos,  TString filename) {


        fCntHistos = 0;

        fHistos = histos;

        fInFile =  new TFile( filename.Data()  );

        if( fInFile->IsOpen() )
        {

            // preparation
            TList*  keyList = fInFile->GetListOfKeys();
            Int_t   objN    = keyList->GetSize();
            TString     histoName;
            TString     s1;

            HasTH2  = false;


            for(int i=0; i< objN; i++)
            {

                TKey*   objKey    = (TKey*) keyList-> At(i);
                TString classType =  objKey->GetClassName();

                // search for  TH2C, TH2S, TH2F ...
                s1 = classType.SubString( "TH2" );
                if ( s1 != "" ) {
                    HasTH2 = true;
                    histoName = objKey->GetName();
                    Search_for_TH2( classType, histoName ); }



                // search for  TH1C, TH1S, TH1F ..
                s1 = classType.SubString( "TH1" );
                if ( s1 != "" ) {

                    histoName = objKey->GetName();
                    Search_for_TH1( classType, histoName  );
                    fCntHistos ++; }
            }
        }
    }


    Int_t   Get_loadedTH1number () { return fCntHistos; }

    Bool_t  Get_hasTH2() { return HasTH2; }

    TH2*    Get_TH2() { return fHisto2d; }




    // According to the type of the 2d histogram,
    // we retrieve it and assign to fHisto2d.
    void Search_for_TH2( TString type, TString name ) {

        if( type == "TH2C" ) {
            h2c = (TH2C*) fInFile->Get( name.Data() ) ;
            fHisto2d =(TH2*) h2c;
        }

        else if( type == "TH2S" ) {
            h2s = (TH2S*) fInFile->Get( name.Data() ) ;
            fHisto2d = (TH2*) h2s->Clone();
        }

        else if( type == "TH2I" ) {
            h2i = (TH2I*) fInFile->Get( name.Data() ) ;
            fHisto2d = (TH2*) h2i;
        }

        else if( type == "TH2F" ) {

            h2f = (TH2F*) fInFile->Get( name.Data() ) ;
            fHisto2d =(TH2*) h2f;
        }

        else if( type == "TH2D" ) {
            h2d = (TH2D*) fInFile->Get( name.Data() ) ;
            fHisto2d = (TH2*) h2d;
        }

    }

     // According to the type of the 1d histogram,
    // we retrieve it and append it to fHistos
    void Search_for_TH1( TString type, TString name) {

        if( type == "TH1C" ) {
            h1c = nullptr;
            h1c = (TH1C*) fInFile->Get( name.Data() ) ;
            fHistos->push_back( (TH1*) h1c  );
        }

        if( type == "TH1S" ) {
            h1s = nullptr;
            h1s = (TH1S*) fInFile->Get( name.Data() ) ;
            fHistos->push_back( (TH1*) h1s  );
        }

        if( type == "TH1I" ) {
            h1i = nullptr;
            h1i = (TH1I*) fInFile->Get( name.Data() ) ;
            fHistos->push_back( (TH1*) h1i  );
        }

        if( type == "TH1F" ) {
            h1f = nullptr;
            h1f = (TH1F*) fInFile->Get( name.Data() ) ;
            fHistos->push_back( (TH1*) h1f  );
        }

         if( type == "TH1D" ) {
            h1d = nullptr;
            // or fInFile->GetObject( name.Data() , h1d );
            h1d = (TH1D*) fInFile->Get( name.Data() ) ;
            fHistos->push_back( (TH1*) h1d );

        }

    }

    ~ReadIn_and_parse() { fInFile->Close(); }






};





//_____________________________________________________________________________
class Calculate_xlow2_xup2 {

private:

    Float_t   diff;
    Float_t   interval;
    Int_t     ratio;
    Int_t     binN2;
    Float_t   xup2;
    Float_t   xlow2;


public:
    // constructor
    Calculate_xlow2_xup2(   Float_t xlow, Float_t xup,
                            Float_t xlow_select, Float_t xup_select,
                            Float_t binWidth ) {

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
            binN2 = ratio;
        }
        else {
            // we need one more bin.
            binN2 = ratio + 1;
            xup2 = binN2 * binWidth + xlow;
        }



        //========================================== xlow2 calculation
        interval = xup2 - xlow_select;

        ratio = interval/binWidth + 0.000005;

        diff = interval - ratio*binWidth;
        // note: due to the truncation, interval alway > (ratio*binWidth)

        if ( diff < 0.00001 ) {
            xlow2 = xlow_select;
            binN2 = ratio;
        }
        else {
            // we need one more bin.
            binN2 = ratio + 1;
            xlow2 = xup2 - binN2 * binWidth;
        }


    } //-------------- end of constructor


    Float_t Get_xup2  () { return xup2;  }
    Float_t Get_xlow2 () { return xlow2; }
    Int_t Get_binN2  ()  { return binN2; }

};



//_____________________________________________________________________________

class Calculate_common_x_range{

    Bool_t  fIsSameDim;
    Float_t fUpperBound;
    Float_t fLowerBound;
    Float_t fXpick1;
    Float_t fXpick2;

public:

    Calculate_common_x_range( TH1* h1, TH1* h2, float xpick1, float xpick2 ) {

        fXpick1 = xpick1;
        fXpick2 = xpick2;

        //check whether we have the same dimensions
        Float_t h1xmin = h1->GetXaxis()->GetXmin();
        Float_t h1xmax = h1->GetXaxis()->GetXmax();
        Float_t h2xmin = h2->GetXaxis()->GetXmin();
        Float_t h2xmax = h2->GetXaxis()->GetXmax();


        Bool_t check1 = ( h1xmin == h2xmin );
        Bool_t check2 = ( h1xmax == h2xmax );
        fIsSameDim = ( check1&& check2 );

        if( fIsSameDim == false ){

            // find out the smallest one amont h1xmax, h2xmax, and xpick2
            vector<Float_t> upper;
            upper.push_back( h1xmax );
            upper.push_back( h2xmax );
            upper.push_back( xpick2 );

            fUpperBound = 1E+38;
            for( Int_t i =0; i< upper.size(); i++ ) {
                if( upper[i] < fUpperBound ) { fUpperBound = upper[i]; }
            }

            // find out the largest one amont h1xmin, h2xmin, and xpick1
            vector<Float_t> lower;
            lower.push_back( h1xmin );
            lower.push_back( h2xmin );
            lower.push_back( xpick1 );
            fLowerBound = -1E+38;
            for( Int_t i =0; i< lower.size(); i++ ) {
                if( lower[i] > fLowerBound ) { fLowerBound = lower[i]; }
            }
        }


    }


    Float_t Get_upper_bound ( ) {
        if( fIsSameDim ){   return fXpick2;  }
        else { return fUpperBound;}
    }

    Float_t Get_lower_bound ( ) {
        if( fIsSameDim ){ return fXpick1; }
        else { return fLowerBound;}
    }

    Bool_t Is_same_dim () { return fIsSameDim; }

};



//_____________________________________________________________________________

class Calculate_fraction_bg{

private:
    TH2*    fHisto2d;
    TH1D*   fFullYProj;
    TH1D*   fGatedX_ProjY;
    TH1D*   fGatedX_ProjY_bg;

    TH1D*   fFullXProj;
    TH1D*   fGatedY_ProjX;
    TH1D*   fGatedY_ProjX_bg;
    Bool_t  toProjonY;

public:

    TH1* get_histogram( ) {
        /* according to gating on which axis, we retrun the corresponding histogram */

        if( toProjonY ) { return (TH1*) fGatedX_ProjY_bg->Clone(); }
        else {  return (TH1*) fGatedY_ProjX_bg->Clone(); }

    }

    // constructor:
    Calculate_fraction_bg ( TH2* histo2d,
                            Float_t g_range1,
                            Float_t g_range2,
                            Float_t bg_value,
                            Bool_t  gating_on_x_proj_to_y ) {
        /*  NOTE:
            we will create hitsogram here,
            to retreive, use get_hitogram()
        */


        fHisto2d = histo2d;
        toProjonY = gating_on_x_proj_to_y;


        if ( gating_on_x_proj_to_y == true ) {

        /*  this part corresponds to key 'p' :
            gating on x then project to y
        */




            fFullYProj  = fHisto2d->ProjectionY() ;

            float xMax = TMath::Max( g_range1, g_range2 );
            float xMin = TMath::Min( g_range1, g_range2 );
            Int_t binx1 = fHisto2d->GetXaxis()-> FindBin(xMin);
            Int_t binx2 = fHisto2d->GetXaxis()-> FindBin(xMax);
            fGatedX_ProjY = fHisto2d->ProjectionY( "gating_on_x_proj_to_y", binx1, binx2 );

            fGatedX_ProjY_bg = (TH1D*) fGatedX_ProjY->Clone();
            fGatedX_ProjY_bg -> Reset(); // to make counts all zero.


            // doing the calculations

            Float_t ratio;
            Float_t C_x;
            Float_t C_total ;
            Int_t  gate_range;
            Int_t binN;



            C_total = fFullYProj->GetEntries();
            gate_range = binx2- binx1 + 1;
            binN = fGatedX_ProjY->GetNbinsX();


            for( Int_t i=0; i<= binN; i++ ) {

                C_x = fFullYProj-> GetBinContent(i);

                ratio = C_x / C_total;

                Float_t yyy = fGatedX_ProjY-> GetBinContent(i); // gated, but no bg substraction.

                yyy = yyy - ratio * bg_value * gate_range ;  // doing bg substraction.

                fGatedX_ProjY_bg->SetBinContent(i, yyy );

                //printf( "y before = %5f , y after = %5f, bg = %5f\n", fGatedX_ProjY-> GetBinContent(i), yyy, ratio * bg_value * gate_range );

            }




        }
       else {

            /*  this part corresponds to key 'P'
                gating on y then project to x
             */

            fFullXProj  = fHisto2d->ProjectionX() ;

            float yMax = TMath::Max( g_range1, g_range2 );
            float yMin = TMath::Min( g_range1, g_range2 );
            Int_t biny1 = fHisto2d->GetYaxis()-> FindBin(yMin);
            Int_t biny2 = fHisto2d->GetYaxis()-> FindBin(yMax);
            fGatedY_ProjX = fHisto2d->ProjectionX( "gating_on_y_proj_to_x", biny1, biny2 );

            fGatedY_ProjX_bg = (TH1D*) fGatedY_ProjX->Clone();
            fGatedY_ProjX_bg -> Reset(); // to make counts all zero.

            Float_t ratio;
            Float_t C_x;
            Float_t C_total ;
            Int_t  gate_range;
            Int_t binN;



            C_total = fFullXProj->GetEntries();
            gate_range = biny2- biny1 + 1;
            binN = fGatedY_ProjX->GetNbinsX();

            for( Int_t i=0; i<= binN; i++ ) {

                C_x = fFullXProj-> GetBinContent(i);

                ratio = C_x / C_total;

                Float_t yyy = fGatedY_ProjX-> GetBinContent(i); // gated, but no bg substraction.

                yyy = yyy - ratio * bg_value * gate_range ;  // doing bg substraction.

                fGatedY_ProjX_bg->SetBinContent(i, yyy );


        }

    }

    } //====================================== end of constructor

};




//_____________________________________________________________________________



class Calculate_addSub_bg{

private:
    TH2*    fHisto2d;
    TH1D*   fAdd;
    TH1D*   fSub;
    TH1D*   fGatedX_ProjY_bg;
    TH1D*   fGatedY_ProjX_bg;

    Float_t*  fAddGate_1;
    Float_t*  fAddGate_2;
    Float_t*  fSubGate_1;
    Float_t*  fSubGate_2;

    Bool_t  toProjonY;

public:

    TH1* get_histogram( ) {
        /* according to gating on which axis, we retrun the corresponding histogram */

        if( toProjonY ) { return (TH1*) fGatedX_ProjY_bg->Clone(); }
        else {  return (TH1*) fGatedY_ProjX_bg->Clone(); }

    }




    // constructor:
    Calculate_addSub_bg ( TH2* histo2d,
                            Float_t add_range1,
                            Float_t add_range2,
                            Float_t sub_range1,
                            Float_t sub_range2,
                            Bool_t  gating_on_x_proj_to_y ) {
        /*  NOTE:
            we will create hitsogram here,
            to retreive, use get_hitogram()
        */


        fHisto2d = histo2d;
        toProjonY = gating_on_x_proj_to_y;


        if ( gating_on_x_proj_to_y == true ) {

        /*  this part corresponds to key 'p' :
            gating on x then project to y
        */

            Float_t xMax;
            Float_t xMin;
            Int_t   binx1;
            Int_t   binx2;
            Int_t   rAdd, rSub;
            Float_t wSub;

            xMax = TMath::Max( add_range1, add_range2 );
            xMin = TMath::Min( add_range1, add_range2 );
            binx1 = fHisto2d->GetXaxis()-> FindBin(xMin);
            binx2 = fHisto2d->GetXaxis()-> FindBin(xMax);
            rAdd = binx2 - binx1 + 1;
            fAdd = fHisto2d->ProjectionY( "AddgX_projtoY", binx1, binx2 );

            xMax = TMath::Max( sub_range1, sub_range2 );
            xMin = TMath::Min( sub_range1, sub_range2 );
            binx1 = fHisto2d->GetXaxis()-> FindBin(xMin);
            binx2 = fHisto2d->GetXaxis()-> FindBin(xMax);
            rSub = binx2 - binx1 + 1;
            fSub = fHisto2d->ProjectionY( "SubgX_projtoY", binx1, binx2 );


            wSub = static_cast<float>( rAdd )/ ( rSub );
            fGatedX_ProjY_bg = (TH1D*) fAdd->Clone();
            fGatedX_ProjY_bg->Reset();
            fGatedX_ProjY_bg->Add( fAdd, fSub, 1, -wSub );






        }
       else {

            /*  this part corresponds to key 'P'
                gating on y then project to x
             */

            Float_t yMax;
            Float_t yMin;
            Int_t   biny1;
            Int_t   biny2;
            Int_t   rAdd, rSub;
            Float_t   wSub;

            yMax = TMath::Max( add_range1, add_range2 );
            yMin = TMath::Min( add_range1, add_range2 );
            biny1 = fHisto2d->GetYaxis()-> FindBin(yMin);
            biny2 = fHisto2d->GetYaxis()-> FindBin(yMax);
            rAdd = biny2 - biny1 + 1;
            fAdd = fHisto2d->ProjectionX( "AddgY_projtoX", biny1, biny2 );

            yMax = TMath::Max( sub_range1, sub_range2 );
            yMin = TMath::Min( sub_range1, sub_range2 );
            biny1 = fHisto2d->GetYaxis()-> FindBin(yMin);
            biny2 = fHisto2d->GetYaxis()-> FindBin(yMax);
            rSub = biny2 - biny1 + 1;
            fSub = fHisto2d->ProjectionX( "SubgY_projtoX", biny1, biny2 );

            wSub = static_cast<float>( rAdd )/ ( rSub );
            fGatedY_ProjX_bg = (TH1D*) fAdd->Clone();
            fGatedY_ProjX_bg->Reset();
            fGatedY_ProjX_bg->Add( fAdd, fSub, 1, -wSub );

        }

    } //====================================== end of constructor

};

//_____________________________________________________________________________


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







