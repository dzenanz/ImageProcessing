/* ============================================================================
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "ItkBinaryWatershedLabeled.h"

//thresholding filter
#include "itkSignedMaurerDistanceMapImageFilter.h"
#include "itkInvertIntensityImageFilter.h"
#include "itkMorphologicalWatershedFromMarkersImageFilter.h"
#include "itkMaskImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"


#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"



// ImageProcessing Plugin
#include "ImageProcessing/ImageProcessingHelpers.hpp"
#include "SIMPLib/ITK/itkBridge.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ItkBinaryWatershedLabeled::ItkBinaryWatershedLabeled()
: m_SelectedCellArrayPath("", "", "")
, m_PeakTolerance(1.0)
, m_NewCellArrayName("BinaryWatershedLabeled")
, m_SelectedCellArray(nullptr)
, m_NewCellArray(nullptr)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ItkBinaryWatershedLabeled::~ItkBinaryWatershedLabeled() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ItkBinaryWatershedLabeled::setupFilterParameters()
{
  FilterParameterVector parameters;
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::RequiredArray));
  {
    DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(SIMPL::TypeNames::Bool, 1, AttributeMatrix::Category::Any);
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Attribute Array to Watershed", SelectedCellArrayPath, FilterParameter::RequiredArray, ItkBinaryWatershedLabeled, req));
  }
  parameters.push_back(SIMPL_NEW_FLOAT_FP("Peak Noise Tolerance", PeakTolerance, FilterParameter::Parameter, ItkBinaryWatershedLabeled));
  parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Watershed Array", NewCellArrayName, FilterParameter::CreatedArray, ItkBinaryWatershedLabeled));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ItkBinaryWatershedLabeled::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedCellArrayPath( reader->readDataArrayPath( "SelectedCellArrayPath", getSelectedCellArrayPath() ) );
  setPeakTolerance( reader->readValue( "PeakTolerance", getPeakTolerance() ) );
  setNewCellArrayName( reader->readString( "NewCellArrayName", getNewCellArrayName() ) );
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ItkBinaryWatershedLabeled::initialize()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ItkBinaryWatershedLabeled::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);
  DataArrayPath tempPath;

  QVector<size_t> dims(1, 1);
  m_SelectedCellArrayPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<bool>, AbstractFilter>(this, getSelectedCellArrayPath(), dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if(nullptr != m_SelectedCellArrayPtr.lock())                            /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  { m_SelectedCellArray = m_SelectedCellArrayPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
  if(getErrorCondition() < 0) { return; }

  ImageGeom::Pointer image = getDataContainerArray()->getDataContainer(getSelectedCellArrayPath().getDataContainerName())->getPrereqGeometry<ImageGeom, AbstractFilter>(this);
  if(getErrorCondition() < 0 || nullptr == image.get()) { return; }

  tempPath.update(getSelectedCellArrayPath().getDataContainerName(), getSelectedCellArrayPath().getAttributeMatrixName(), getNewCellArrayName() );
  m_NewCellArrayPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<uint32_t>, AbstractFilter, ImageProcessingConstants::DefaultPixelType>(this, tempPath, 0, dims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if(nullptr != m_NewCellArrayPtr.lock())                       /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  { m_NewCellArray = m_NewCellArrayPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ItkBinaryWatershedLabeled::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck(); // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted(); // We are done preflighting this filter
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ItkBinaryWatershedLabeled::execute()
{
  QString ss;
  dataCheck();
  if(getErrorCondition() < 0)
  {
    setErrorCondition(-11000);
    ss = QObject::tr("DataCheck did not pass during execute");
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return;
  }

  DataContainer::Pointer m = getDataContainerArray()->getDataContainer(getSelectedCellArrayPath().getDataContainerName());
  QString attrMatName = getSelectedCellArrayPath().getAttributeMatrixName();

  //get utilities
  typedef ItkBridge<bool> BoolBridgeType;
  typedef ItkBridge<float> FloatBridgeType;
  typedef ItkBridge<uint32_t> LabelBridgeType;

  //wrap input
  BoolBridgeType::ScalarImageType::Pointer inputImage = BoolBridgeType::CreateItkWrapperForDataPointer(m, attrMatName, m_SelectedCellArray);

  //compute distance map
  typedef itk::SignedMaurerDistanceMapImageFilter<BoolBridgeType::ScalarImageType, FloatBridgeType::ScalarImageType> DistanceMapType;
  DistanceMapType::Pointer distanceMap = DistanceMapType::New();
  distanceMap->SetInsideIsPositive(true);
  distanceMap->SetInput(inputImage);
  try
  {
    distanceMap->Update();
  }
  catch( itk::ExceptionObject& err )
  {
    setErrorCondition(-5);
    QString ss = QObject::tr("Failed to execute itk::KMeans filter. Error Message returned from ITK:\n   %1").arg(err.GetDescription());
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }

  //find maxima in distance map (ultimate points)
  std::vector<FloatBridgeType::ScalarImageType::IndexType> peakLocations = ImageProcessing::LocalMaxima<FloatBridgeType::ScalarImageType>::Find(distanceMap->GetOutput(), m_PeakTolerance, true);

  //create labeled image from peaks
  typedef itk::Image<uint32_t, FloatBridgeType::ScalarImageType::ImageDimension> LabelImageType;
  LabelImageType::Pointer seedLabels = LabelImageType::New();
  LabelImageType::RegionType region = inputImage->GetLargestPossibleRegion();
  seedLabels->SetRegions(region);
  seedLabels->Allocate();
  seedLabels->FillBuffer(0);
  for(size_t i = 0; i < peakLocations.size(); i++)
  {
    seedLabels->SetPixel(peakLocations[i], i + 1);
  }
  seedLabels->CopyInformation(inputImage);

  //invert distance map as gradient for watershed
  typedef itk::InvertIntensityImageFilter< FloatBridgeType::ScalarImageType, FloatBridgeType::ScalarImageType > InvertType;
  InvertType::Pointer invert = InvertType::New();
  invert->SetInput(distanceMap->GetOutput());
  invert->SetMaximum(0);

  //set up seeded watershed
  typedef itk::MorphologicalWatershedFromMarkersImageFilter< FloatBridgeType::ScalarImageType, LabelImageType > WatershedType;
  WatershedType::Pointer watershed = WatershedType::New();
  watershed->SetInput1(invert->GetOutput());
  watershed->SetInput2(seedLabels);
  watershed->SetMarkWatershedLine(false);

  //mask watershed output (we only want things that were originally in the)
  typedef itk::MaskImageFilter<LabelImageType, BoolBridgeType::ScalarImageType, LabelBridgeType::ScalarImageType> MaskType;
  MaskType::Pointer mask = MaskType::New();
  mask->SetInput(watershed->GetOutput());
  mask->SetMaskImage(inputImage);

//  //threshold all labels into boolean array
//  typedef itk::BinaryThresholdImageFilter< LabelImageType, BoolBridgeType::ScalarImageType > ThresholdType;
//  ThresholdType::Pointer threshold = ThresholdType::New();
//  threshold->SetInput(mask->GetOutput());
//  threshold->SetLowerThreshold(1);
//  threshold->SetInsideValue(true);
//  threshold->SetOutsideValue(false);

  //wrap output
  LabelBridgeType::SetITKFilterOutput(mask->GetOutput(), m_NewCellArrayPtr.lock());
  try
  {
    mask->Update();
  }
  catch( itk::ExceptionObject& err )
  {
    setErrorCondition(-5);
    QString ss = QObject::tr("Failed to execute itk::KMeans filter. Error Message returned from ITK:\n   %1").arg(err.GetDescription());
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ItkBinaryWatershedLabeled::newFilterInstance(bool copyFilterParameters) const
{
  ItkBinaryWatershedLabeled::Pointer filter = ItkBinaryWatershedLabeled::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ItkBinaryWatershedLabeled::getCompiledLibraryName() const
{return ImageProcessingConstants::ImageProcessingBaseName;}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ItkBinaryWatershedLabeled::getGroupName() const
{return SIMPL::FilterGroups::Unsupported;}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ItkBinaryWatershedLabeled::getUuid()
{
  return QUuid("{76fd1b13-5feb-5338-8d7f-b3b935ff3f22}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ItkBinaryWatershedLabeled::getSubGroupName() const
{return "Misc";}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ItkBinaryWatershedLabeled::getHumanLabel() const
{ return "Binary Watershed Labeled (ImageProcessing)"; }

