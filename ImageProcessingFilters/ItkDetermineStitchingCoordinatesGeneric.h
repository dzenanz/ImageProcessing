/*
 * Your License or Copyright Information can go here
 */

#pragma once

#include "ImageProcessing/ImageProcessingConstants.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/StringDataArray.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

#include "ImageProcessing/ImageProcessingDLLExport.h"

/**
 * @class DetermineStitchingCoordinatesGeneric DetermineStitchingCoordinatesGeneric.h ZeissImport/ZeissImportFilters/DetermineStitchingCoordinatesGeneric.h
 * @brief
 * @author
 * @date
 * @version 1.0
 */
class ImageProcessing_EXPORT ItkDetermineStitchingCoordinatesGeneric : public AbstractFilter
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(ItkDetermineStitchingCoordinatesGeneric SUPERCLASS AbstractFilter)
  PYB11_PROPERTY(DataArrayPath AttributeMatrixName READ getAttributeMatrixName WRITE setAttributeMatrixName)
  PYB11_PROPERTY(int ImportMode READ getImportMode WRITE setImportMode)
  PYB11_PROPERTY(int xTileDim READ getxTileDim WRITE setxTileDim)
  PYB11_PROPERTY(int yTileDim READ getyTileDim WRITE setyTileDim)
  PYB11_PROPERTY(float OverlapPer READ getOverlapPer WRITE setOverlapPer)
  PYB11_PROPERTY(bool UseZeissMetaData READ getUseZeissMetaData WRITE setUseZeissMetaData)
  PYB11_PROPERTY(DataArrayPath MetaDataAttributeMatrixName READ getMetaDataAttributeMatrixName WRITE setMetaDataAttributeMatrixName)
  PYB11_PROPERTY(QString TileCalculatedInfoAttributeMatrixName READ getTileCalculatedInfoAttributeMatrixName WRITE setTileCalculatedInfoAttributeMatrixName)
  PYB11_PROPERTY(QString StitchedCoordinatesArrayName READ getStitchedCoordinatesArrayName WRITE setStitchedCoordinatesArrayName)
  PYB11_PROPERTY(QString StitchedArrayNames READ getStitchedArrayNames WRITE setStitchedArrayNames)

public:
  SIMPL_SHARED_POINTERS(ItkDetermineStitchingCoordinatesGeneric)
  SIMPL_FILTER_NEW_MACRO(ItkDetermineStitchingCoordinatesGeneric)
  SIMPL_TYPE_MACRO_SUPER_OVERRIDE(ItkDetermineStitchingCoordinatesGeneric, AbstractFilter)

  ~ItkDetermineStitchingCoordinatesGeneric() override;

  /* Place your input parameters here using the DREAM3D macros to declare the Filter Parameters
   * or other instance variables
   */
  // SIMPL_FILTER_PARAMETER(QString, ImagePrefix)
  /* If you declare a filter parameter above then you MUST create a Q_PROPERTY for that FilterParameter */
  // Q_PROPERTY(QString ImagePrefix READ getImagePrefix WRITE setImagePrefix)

  /* Here is another example of declaring an integer FilterParameter */
  // SIMPL_FILTER_PARAMETER(int, ImageSize)
  // Q_PROPERTY(int ImageSize READ getImageSize WRITE setImageSize)

  SIMPL_FILTER_PARAMETER(DataArrayPath, AttributeMatrixName)
  Q_PROPERTY(DataArrayPath AttributeMatrixName READ getAttributeMatrixName WRITE setAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(int, ImportMode)
  Q_PROPERTY(int ImportMode READ getImportMode WRITE setImportMode)

  SIMPL_FILTER_PARAMETER(int, xTileDim)
  Q_PROPERTY(int xTileDim READ getxTileDim WRITE setxTileDim)

  SIMPL_FILTER_PARAMETER(int, yTileDim)
  Q_PROPERTY(int yTileDim READ getyTileDim WRITE setyTileDim)

  SIMPL_FILTER_PARAMETER(float, OverlapPer)
  Q_PROPERTY(float OverlapPer READ getOverlapPer WRITE setOverlapPer)

  SIMPL_FILTER_PARAMETER(bool, UseZeissMetaData)
  Q_PROPERTY(bool UseZeissMetaData READ getUseZeissMetaData WRITE setUseZeissMetaData)

  SIMPL_FILTER_PARAMETER(DataArrayPath, MetaDataAttributeMatrixName)
  Q_PROPERTY(DataArrayPath MetaDataAttributeMatrixName READ getMetaDataAttributeMatrixName WRITE setMetaDataAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, TileCalculatedInfoAttributeMatrixName)
  Q_PROPERTY(QString TileCalculatedInfoAttributeMatrixName READ getTileCalculatedInfoAttributeMatrixName WRITE setTileCalculatedInfoAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(QString, StitchedCoordinatesArrayName)
  Q_PROPERTY(QString StitchedCoordinatesArrayName READ getStitchedCoordinatesArrayName WRITE setStitchedCoordinatesArrayName)

  SIMPL_FILTER_PARAMETER(QString, StitchedArrayNames)
  Q_PROPERTY(QString StitchedArrayNames READ getStitchedArrayNames WRITE setStitchedArrayNames)

  /**
   * @brief getCompiledLibraryName Returns the name of the Library that this filter is a part of
   * @return
   */
  const QString getCompiledLibraryName() const override;

  /**
   * @brief This returns a string that is displayed in the GUI. It should be readable
   * and understandable by humans.
   */
  const QString getHumanLabel() const override;

  /**
   * @brief This returns the group that the filter belonds to. You can select
   * a different group if you want. The string returned here will be displayed
   * in the GUI for the filter
   */
  const QString getGroupName() const override;

  /**
   * @brief This returns a string that is displayed in the GUI and helps to sort the filters into
   * a subgroup. It should be readable and understandable by humans.
   */
  const QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  const QUuid getUuid() override;

  /**
   * @brief This method will instantiate all the end user settable options/parameters
   * for this filter
   */
  void setupFilterParameters() override;

  /**
   * @brief This method will read the options from a file
   * @param reader The reader that is used to read the options from a file
   * @param index The index to read the information from
   */
  void readFilterParameters(AbstractFilterParametersReader* reader, int index) override;

  /**
   * @brief Reimplemented from @see AbstractFilter class
   */
  void execute() override;

  /**
   * @brief This function runs some sanity checks on the DataContainer and inputs
   * in an attempt to ensure the filter can process the inputs.
   */
  void preflight() override;

  /**
   * @brief newFilterInstance Returns a new instance of the filter optionally copying the filter parameters from the
   * current filter to the new instance.
   * @param copyFilterParameters
   * @return
   */
  AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

signals:
  /**
   * @brief updateFilterParameters This is emitted when the filter requests all the latest Filter Parameters need to be
   * pushed from a user facing control such as the FilterParameter Widget
   * @param filter The filter to push the values into
   */
  void updateFilterParameters(AbstractFilter* filter);

  /**
   * @brief parametersChanged This signal can be emitted when any of the filter parameters are changed internally.
   */
  void parametersChanged();

  /**
   * @brief preflightAboutToExecute Emitted just before the dataCheck() is called. This can change if needed.
   */
  void preflightAboutToExecute();

  /**
   * @brief preflightExecuted Emitted just after the dataCheck() is called. Typically. This can change if needed.
   */
  void preflightExecuted();

protected:
  ItkDetermineStitchingCoordinatesGeneric();

  QVector<qint32> extractIntegerValues(QString arrayName);
  QVector<float> extractFloatValues(QString arrayName);
  QVector<float> extractGlobalIndices(QString DataArrayName, QString resolution);

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

private:
  //    DEFINE_DATAARRAY_VARIABLE(QVector<IDataArray::Pointer>, PointerList)

  QVector<ImageProcessingConstants::DefaultPixelType*> m_PointerList;
  DEFINE_DATAARRAY_VARIABLE(ImageProcessingConstants::DefaultPixelType, SelectedCellArray)
  DEFINE_DATAARRAY_VARIABLE(float, StitchedCoordinates)
  StringDataArray::WeakPointer m_DataArrayNamesForStitchedCoordinatesPtr;
  //DEFINE_DATAARRAY_VARIABLE(StringDataArray::WeakPointer, DataArrayNamesForStichedCoordinates);
  //DEFINE_DATAARRAY_VARIABLE(QString, DataArrayNamesForStitchedCoordinates);

public:
  ItkDetermineStitchingCoordinatesGeneric(const ItkDetermineStitchingCoordinatesGeneric&) = delete; // Copy Constructor Not Implemented
  ItkDetermineStitchingCoordinatesGeneric(ItkDetermineStitchingCoordinatesGeneric&&) = delete;      // Move Constructor Not Implemented
  ItkDetermineStitchingCoordinatesGeneric& operator=(const ItkDetermineStitchingCoordinatesGeneric&) = delete; // Copy Assignment Not Implemented
  ItkDetermineStitchingCoordinatesGeneric& operator=(ItkDetermineStitchingCoordinatesGeneric&&) = delete;      // Move Assignment Not Implemented
};

