#include "dialog.h"
#include "ui_dialog.h"
#include "formanalyzer.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    AS::Shape tshape2, tshape1;

    tshape2.coordinates.push_back( AS::Coordinate( 0,0,0 ) );
    tshape2.coordinates.push_back( AS::Coordinate( 10,8,0 ) );
    tshape2.coordinates.push_back( AS::Coordinate( 20,0,0 ) );

    tshape1.coordinates.push_back( AS::Coordinate( 0,0,0 ) );
    tshape1.coordinates.push_back( AS::Coordinate( 6,8,0 ) );
    tshape1.coordinates.push_back( AS::Coordinate( 20,0,0 ) );

    FormAnalyzer analyzer;
    //analyzer.fillRelativePositions( tshape1 );
    //analyzer.fillRelativePositions( tshape2 );
    analyzer.preAnalyzeShape( tshape1 );
    analyzer.preAnalyzeShape( tshape2 );


    float val = analyzer.compareShapes(tshape1, tshape2 );

}

Dialog::~Dialog()
{
    delete ui;
}
