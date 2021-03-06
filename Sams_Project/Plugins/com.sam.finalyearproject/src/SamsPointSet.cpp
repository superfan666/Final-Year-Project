#include "SamsPointSet.h"

#include <mitkInteractionConst.h>

void SamsPointSet::SetSamsView(Sams_View * samsView) {
  this->samsView = samsView;
}

void SamsPointSet::ExecuteOperation(mitk::Operation * operation) {
  switch (operation->GetOperationType()) {
    case mitk::OpNOTHING:
      if (DEBUGGING)
        std::cout << "Nothing..." << std::endl;
      break;

    case mitk::OpINSERT://inserts the point at the given position and selects it.
      if (DEBUGGING)
        std::cout << "Inserting a point..." << std::endl;
      break;

    case mitk::OpMOVE://moves the point given by index
      if (DEBUGGING)
        std::cout << "Moving a point..." << std::endl;
      break;

    case mitk::OpREMOVE://removes the point at given by position
      if (DEBUGGING)
        std::cout << "Removing a point..." << std::endl;
      break;

    case mitk::OpSELECTPOINT://select the given point
      if (DEBUGGING)
        std::cout << "Selecting a point..." << std::endl;
      OnPointSetChange();
      break;

    case mitk::OpDESELECTPOINT://unselect the given point
      if (DEBUGGING)
        std::cout << "Deselecting a point..." << std::endl;
      OnPointSetChange();
      break;

    case mitk::OpSETPOINTTYPE:
      if (DEBUGGING)
        std::cout << "Setting point type..." << std::endl;
      break;

    case mitk::OpMOVEPOINTUP: // swap content of point with ID pointOp->GetIndex() with the point preceding it in the container // move point position within the pointset
      if (DEBUGGING)
        std::cout << "Moving point up..." << std::endl;
      break;
    
    case mitk::OpMOVEPOINTDOWN: // move point position within the pointset
      if (DEBUGGING)
        std::cout << "Moving point down..." << std::endl;
      break;

    default:
      if (DEBUGGING)
        std::cout << "Unknown..." << std::endl;
      break;
  }

  // Do the normal execution afterwards.
  mitk::PointSet::ExecuteOperation(operation);
}

void SamsPointSet::OnPointSetChange() {
  if (DEBUGGING)
    std::cout << "Point set changed." << std::endl;
  samsView->PointSetChanged(this);
}