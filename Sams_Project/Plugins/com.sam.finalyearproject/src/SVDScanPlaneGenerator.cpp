#include "SVDScanPlaneGenerator.h"

#include "Util.h"
#include "NoPointsException.h"

#include <mitkVector.h>
#include <mitkImagePixelReadAccessor.h>
#include <vnl/algo/vnl_svd.h>
#include <vcl_iostream.h>

// Loading bar
#include <mitkProgressBar.h>

SVDScanPlaneGenerator::SVDScanPlaneGenerator() {
  this->threshold = 0.5;
}

/**
  * Set the uncertainty representing the volume to be scanned.
  */
void SVDScanPlaneGenerator::setUncertainty(mitk::Image::Pointer uncertainty) {
  this->uncertainty = uncertainty;
  this->uncertaintyHeight = uncertainty->GetDimension(0);
  this->uncertaintyWidth = uncertainty->GetDimension(1);
  this->uncertaintyDepth = uncertainty->GetDimension(2);
}

/**
  * SVD uses all of the points that are worse than a given threshold to calculate the scan plane.
  * This allows it to focus on points that are particularly bad.
  * This method sets that threshold.
  */
void SVDScanPlaneGenerator::setThreshold(double threshold) {
  this->threshold = threshold;
}

/**
  * Sets whether we should ignore zeros when calculating the next scan plane.
  * If true then voxels that are zero are not included in the SVD computation.
  */
void SVDScanPlaneGenerator::setIgnoreZeros(bool ignoreZeros) {
  this->ignoreZeros = ignoreZeros;
}

/**
  * Uses SVD to calculate the next best scan plane.
  */
vtkSmartPointer<vtkPlane> SVDScanPlaneGenerator::calculateBestScanPlane() {
  mitk::ProgressBar::GetInstance()->AddStepsToDo(3);
  
  // Get all points worse than specified threshold.
  mitk::PointSet::Pointer pointSet = pointsBelowThreshold(threshold);

  // If there are no points then throw an Exception.
  if (pointSet->GetSize() == 0) {
    mitk::ProgressBar::GetInstance()->Progress(3);
    throw NoPointsException();
  }

  mitk::ProgressBar::GetInstance()->Progress();

  // Demean them.
  vnl_matrix<mitk::ScalarType> demeanedMatrix(pointSet->GetSize(), 3);

  mitk::Point3D centroid;
  calculateCentroid(pointSet, centroid);

  int pointTotal = pointSet->GetSize(0);
  for (int i = 0; i< pointTotal; i++) {
    mitk::Point3D point = pointSet->GetPoint(i);
    demeanedMatrix[i][0] = point[0] - centroid[0];
    demeanedMatrix[i][1] = point[1] - centroid[1];
    demeanedMatrix[i][2] = point[2] - centroid[2];
  }

  mitk::ProgressBar::GetInstance()->Progress();

  // Run SVD.
  vnl_svd<mitk::ScalarType> svd(demeanedMatrix, 0.0);
  // vnl_vector<mitk::ScalarType> normal = svd.nullvector();
  // equivalent to
  // vnl_vector<mitk::ScalarType> normal = svd.V().get_column(2);
  // should actually be!!
  vnl_vector<mitk::ScalarType> normal = svd.U().get_row(2);
  if (normal[0] < 0) {
    normal = -normal;
  }
  // also we have these!
  // vnl_vector<mitk::ScalarType> x = svd.U().get_row(1);
  // vnl_vector<mitk::ScalarType> t = svd.U().get_row(2);

  mitk::ProgressBar::GetInstance()->Progress();

  // The plane consists of the centroid and normal.
  vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
  plane->SetOrigin(centroid[0], centroid[1], centroid[2]);
  plane->SetNormal(normal[0], normal[1], normal[2]);
  return plane;
}

/**
  * Get a list of all points in the uncertainty data that are below a given threshold.
  */
mitk::PointSet::Pointer SVDScanPlaneGenerator::pointsBelowThreshold(double threshold) {
  mitk::ProgressBar::GetInstance()->AddStepsToDo(uncertaintyHeight);

  mitk::PointSet::Pointer pointSet = mitk::PointSet::New();

  try  {
    // See if the uncertainty data is available to be read.
    mitk::ImagePixelReadAccessor<double, 3> readAccess(this->uncertainty);
    unsigned int pointCount = 0;
    for (unsigned int x = 0; x < uncertaintyHeight; x++) {
      for (unsigned int y = 0; y < uncertaintyWidth; y++) {
        for (unsigned int z = 0; z < uncertaintyDepth; z++) {
          itk::Index<3> index;
          index[0] = x;
          index[1] = y;
          index[2] = z;
          double indexUncertainty = readAccess.GetPixelByIndex(index);

          // If the value is below the threshold add it to the set.
          if (indexUncertainty < threshold) {
            // If we're ignoring zeros and it is zero then skip it.
            if (ignoreZeros && indexUncertainty == 0.0) {
              continue;
            }
            mitk::Point3D point;
            point[0] = x;
            point[1] = y;
            point[2] = z;
            pointSet->InsertPoint(pointCount, point);
            pointCount++;
          }
        }
      }
      mitk::ProgressBar::GetInstance()->Progress();
    }
  }
  catch (mitk::Exception & e) {
    cerr << "Hmmm... it appears we can't get read access to the uncertainty image. Maybe it's gone? Maybe it's type isn't double? (I've assumed it is)" << e << endl;
  }

  return pointSet;
}

/**
  * Calculates the center of a set of points.
  * TODO: Perhaps weight points based on uncertainty to turn this into a 'center of mass'.
  */
void SVDScanPlaneGenerator::calculateCentroid(mitk::PointSet::Pointer pointSet, mitk::Point3D & centroid) {
  // If the point set is null, return (-1, -1, -1)
  if (pointSet.IsNull()) {
    centroid[0] = -1;
    centroid[1] = -1;
    centroid[2] = -1;
    return;
  }

  // If the point set isn't null, calculate the average point.
  int pointTotal = pointSet->GetSize(0);
  centroid[0] = centroid[1] = centroid[2] = 0.0;
  for (int i = 0; i < pointTotal; i++) {
    mitk::Point3D point = pointSet->GetPoint(i);
    centroid[0] += point[0];
    centroid[1] += point[1];
    centroid[2] += point[2];
  }

  centroid[0] /= pointTotal;
  centroid[1] /= pointTotal;
  centroid[2] /= pointTotal;
}