/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2021, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Open Design Alliance software pursuant to a license 
//   agreement with Open Design Alliance.
//   Open Design Alliance Copyright (C) 2002-2021 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////


#ifndef _FdField_h_Included_
#define _FdField_h_Included_

#include "RxObject.h"
#include "RxDictionary.h"
#include "DbField.h"
#include "StringArray.h"

class OdDbField;
class OdDbEvalVariant;
class OdFieldValue;
class OdFdFieldEngineImpl;
class OdFdFieldResult;
class OdDbDatabase;

#include "TD_PackPush.h"

/** \details
  This class is an abstract base class that defines the field evaluator
  interface.
  \remarks
  Applications implement evaluators for fields by deriving from this class.
  The evaluator object instantiated from this class is obtained from the field
  evaluator loader using the evaluator ID stored in the field. An evaluator
  class can be implemented for each type of field, or a single evaluator class
  can support multiple types of fields.

  <group Other_Classes>
*/
class DBENT_EXPORT OdFdFieldEvaluator : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdFdFieldEvaluator);
  //DOM-IGNORE-END

  /** \details
    Returns the evaluator IDs supported by this evaluator.
    \remarks
    This function can return more than one evaluator ID if the evaluator
    supports multiple IDs. If more than one ID is returned, the evaluator
    IDs are separated by commas.
  */
  virtual const OdString evaluatorId() const = 0;

  /** \details
    Returns the evaluator ID for the specified field code.
    \param pField [in] Input field for which to return an evaluator ID.
    \remarks
    This version will be called to find an evaluator for a field code that does
    not have an evaluator ID set. Implementations of this function should
    examine the field code. If the field code is recognized by the evaluator and
    can be handled by it, then this function should return the evaluator ID.
  */
  virtual const OdString evaluatorId(OdDbField* pField) const = 0;

  /** \details
    This function is called by the field framework. It calls this method to
    initialize a new field.
    \param pField [in] Input field to initialize.
    \remarks
    This method is called after setting a field code in the field and before
    compile() is called. The evaluator can change the default options that are
    set in the field when it is created. This method will be called only once
    for a field.
  */
  virtual OdResult initialize(OdDbField* pField) const = 0;

// The field framework calls this method to compile a field when the field code
// is changed.

  /** \details
    This function is called by the field framework to compile a field when the
    field code is changed.
    \param pField  [in] Input field to compile.
    \param pDb     [in] Input database pointer.
    \param pResult [in] Input field result object to set the compilation status.
    \remarks
    The evaluator can do any parsing of the field code here and store the parsed
    data in the field. This method will be called only when the field code is
    changed, unlike evaluate(), which is called whenever the field needs to be
    evaluated.
  */
  virtual OdResult compile(OdDbField* pField,
    OdDbDatabase* pDb,
    OdFdFieldResult* pResult) const = 0;

// The field framework calls this method to evaluate a field whenever the field
// needs to be evaluated.

  /** \details
    This function is called by the field framework when the field needs to be
    evaluated.
    \param pField   [in] Input field to evaluate.
    \param nContext [in] Input context in which the field is evaluated; this
                         context flag is passed to the evaluator and can be one
                         of the predefined by the OdDbField::EvalContext
                         enumeration flags or a user-defined context flag.
    \param pDb      [in] Input database pointer, which can be NULL if a database could not be obtained in the current context.
    \param pResult  [in] Input field result object to set the evaluation result and status.
    \remarks
    A field will be evaluated only when the evaluation option set in the field
    matches the context in which the field evaluation is invoked. For example,
    if a field option is set to evaluate only manually, then the field will be
    evaluated only when the user updates the specific field or all fields in the
    drawing manually.

    The evaluator should evaluate the field and set the evaluated value using
    OdFdFieldResult::setFieldValue() and the error status using
    OdFdFieldResult::setEvaluationStatus() in the passed result object, which
    in turn will set them in the field object.

    The passed database pointer may be NULL if the field is not a database
    resident and a database could not be obtained in the current context. If the
    field evaluation requires a database, then this method can set the
    evaluation error status in the field result object and return an error.

    If the evaluation fails for any reason, the previously cached result in the
    field object can be left intact and used as the field value. Alternatively,
    the cached result can be deleted and substituted with an error string (for
    example, "#ERR"), which will be displayed as a field value.
  */
  virtual OdResult evaluate(OdDbField* pField,
    int nContext,
    OdDbDatabase* pDb,
    OdFdFieldResult* pResult) const = 0;

// The field framework calls this method when a field is queried to get the
// evaluated field value as a string.

  /** \details
    This function is called by the field framework when a field is queried to
    get the evaluated field value as a string.
    \param pField    [in] Field whose value is to be formatted.
    \param pszValue [out] Pointer to the string that is the field value formatted as a string.
    \remarks
    The evaluator can implement this method to support custom formatting of
    field values. If the evaluator does not implement this method, the standard
    data types will be formatted using default formats.
  */
  virtual OdResult format(OdDbField* pField, OdString& pszValue) const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for
  OdFdFieldEvaluator object pointers.
*/
typedef OdSmartPtr<OdFdFieldEvaluator> OdFdFieldEvaluatorPtr;

/** \details
  This class is used to load the evaluators required for evaluating fields.
  \remarks
  This class acts as the interface between the field framework and applications
  that implement evaluators. An application that implements evaluators should
  derive from this class and register an object of this class with the field
  engine when the application is loaded. The registered loader is used by the
  engine to obtain the evaluator object. Only one loader object is required for
  an application and it can support multiple evaluators. The loader objects are
  maintained globally and are used by all open documents.

  <group Other_Classes>
*/
class DBENT_EXPORT OdFdFieldEvaluatorLoader : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdFdFieldEvaluatorLoader);
  //DOM-IGNORE-END

  /** \details
    The field engine calls this method to get the evaluator using the evaluator
    ID and uses the evaluator to evaluate the field.
    \param pszEvalId [in] Input evaluator ID for which the evaluator is required.
  */
  virtual OdFdFieldEvaluator* getEvaluator(
    const OdString& pszEvalId) = 0;

  /** \details
    Finds an evaluator for a field that does not have an evaluator ID.
    \param pField     [in] Input field for which to find an evaluator.
    \param pszEvalId [out] Output pointer to receive the evaluator ID.
  */
  virtual OdFdFieldEvaluator * findEvaluator(
    OdDbField* pField,
    OdString& pszEvalId) = 0;
};

/** \details
  This class is used to get notifications when fields in a database are
  evaluated.

  <group Other_Classes>
*/
class DBENT_EXPORT OdFdFieldReactor : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdFdFieldReactor);
  //DOM-IGNORE-END

  /** \details
    Notification function that is called before one or more fields in a database
    are evaluated.
    \param nContext [in] Input evaluation context.
    \param pDb      [in] Input pointer to the database in which the fields are evaluated.
  */
  virtual void beginEvaluateFields(
    int nContext,
    OdDbDatabase* pDb);

  /** \details
    Notification function that is called after one or more fields in a database
    are evaluated.
    \param nContext [in] Input evaluation context.
    \param pDb      [in] Input pointer to the database in which the fields are evaluated.
  */
  virtual void endEvaluateFields(
    int nContext,
    OdDbDatabase* pDb);
};

/** \details
  This template class is a specialization of the OdArray class for
  OdFdFieldReactor object pointers.
*/
typedef OdArray<OdFdFieldReactor*> FieldReactors;

/** \details
  This class manages the evaluation of fields.
  \remarks
  The field engine is a global one-per-session object.

  Field evaluators should register their loader with the engine at
  application startup.

  <group Other_Classes>
*/
class DBENT_EXPORT OdFdFieldEngine : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdFdFieldEngine);
  //DOM-IGNORE-END

  /** \details
    Destructor for objects of the OdFdFieldEngine class. Frees allocated
    resources.
  */
  virtual ~OdFdFieldEngine(){}

  /** \details
    Registers the evaluator loader with the engine.
    \param pLoader [in] Input evaluator loader to register.
    \remarks
    The engine searches the registered evaluator loaders to find the appropriate
    evaluator for evaluating fields. Field evaluators should register the
    evaluator loader with the field engine at application startup.
  */
  virtual void registerEvaluatorLoader(OdFdFieldEvaluatorLoader* pLoader) = 0;

  /** \details
    Unregisters the evaluator loader from the engine.
    \param pLoader [in] Input evaluator loader to unregister.
    \remarks
    Field evaluator modules should unregister the evaluator loader when the
    module is unloaded, before deleting the loader object.
  */
  virtual void unregisterEvaluatorLoader(OdFdFieldEvaluatorLoader* pLoader) = 0;

  /** \details
    Returns the number of registered field evaluator loaders.
    \returns Number of registered field evaluator loaders.
  */
  virtual int evaluatorLoaderCount() const = 0;

  /** \details
    Returns the field evaluator loader at the specified index.
    \param inputIndex [in] Input index for which to get the field evaluator
                           loader; the index is zero based and should be less
                           than the number of registered evaluator loaders.
    \remarks
    This function can be used with evaluatorLoaderCount() to enumerate the
    evaluator loaders.
  */
  virtual OdFdFieldEvaluatorLoader * getEvaluatorLoader(int inputIndex) = 0;

  /** \details
    Returns the field evaluator for the specified evaluator ID.
    \param pszEvalId [in] Input evaluator ID for which to find the field evaluator.
    \returns Field evaluator with the corresponding ID. Returns NULL if a field
    evaluator could not be found for the evaluator ID.
  */
  virtual OdFdFieldEvaluator * getEvaluator (
    const OdString& pszEvalId) = 0;

  /** \details
    Returns an an evaluator for the specified field from the set of registered
    evaluators if the field does not have an evaluator ID.
    \param pField     [in] Input field for which to find an evaluator.
    \param pszEvalId [out] Output pointer to receive the evaluator ID.
    \returns Field evaluator for the specified field, or NULL if a field
    evaluator could not be found.
  */
  virtual OdFdFieldEvaluator * findEvaluator(
    OdDbField* pField,
    OdString& pszEvalId) = 0;

  /** \details
    Adds the field reactor to the field framework.
    \param pReactor [in] Input pointer to the reactor to be added.
  */
  virtual void addFieldReactor(OdFdFieldReactor* pReactor) = 0;

  /** \details
    Removes the field reactor from the field framework.
    \param pReactor [in] Input pointer to the field reactor to remove.
  */
  virtual void removeFieldReactor(OdFdFieldReactor* pReactor) = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for
  OdFdFieldEngine object pointers.
*/
typedef OdSmartPtr<OdFdFieldEngine> OdFdFieldEnginePtr;
#define ODFD_FIELD_ENGINE             OD_T("FieldEngine")

/** \details
  Returns the pointer to the field engine. There is only one field engine object
  per session.
  \returns Pointer to the field engine.
*/
DBENT_EXPORT OdFdFieldEnginePtr oddbGetFieldEngine();

/** \details
  Gets the value format by error of the field engine and applies it to the value
  of the previous evaluation.
  \param sPrevValue [in] Value of the previous evaluation.
  \returns Formatted value of the previous evaluation.
*/
DBENT_EXPORT OdString oddbGetFieldEngineValueByError(const OdString& sPrevValue);

/** \details
  Sets the new value format by error for the field engine.
  \param sValue [in] New value format. Leave the default value to keep the result of the previous evaluation as is.
  \returns Previous value format by error of the field engine.
*/
DBENT_EXPORT OdString oddbSetFieldEngineValueFormatByError(
                          const OdString& sValue = L"%ls"); // value to keep result of previous evaluation

/** \details
  This class is used by evaluators to set the evaluation status and evaluation
  result in a field.
  \remarks
  This object is passed to evaluators during compilation and evaluation. This
  object cannot be instantiated.

  <group Other_Classes>
*/
class DBENT_EXPORT OdFdFieldResult : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdFdFieldResult);
  OdFdFieldResult();
  //DOM-IGNORE-END

  /** \details
    Sets the evaluation result in the field.
    \param pValue [in] Input evaluation to set in the field.
    \remarks
    If pValue is NULL, the cached evaluation is deleted.
  */
  virtual void setFieldValue(const OdFieldValue* pValue) = 0;

  /** \details
    Sets the evaluation status in the field.
    \param errorStatus  [in] Error status.
    \param errorCode    [in] Extended error code or zero for success.
    \param errorMessage [in] Error message.
  */
  virtual void setEvaluationStatus(OdDbField::EvalStatus errorStatus, int errorCode = 0, const OdString& errorMessage = OdString()) = 0;
};

/** \details
  This abstract class is the Field Engine Protocol Extension class. Its purpose
  is to provide services necessary for fields evaluation implemented at the host
  application level.

  <group Other_Classes>
*/
class DBENT_EXPORT OdFdFieldEnginePE : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdFdFieldEnginePE);
  //DOM-IGNORE-END

  /** \details
    Retrieves a sheet set property value.

    \param propValue [out] Resulting property value.
    \param pField     [in] Field defining the requested property, containing at least a "SheetSetPropertyName" data item.
    \param pDb        [in] Database.

    \returns eOk if the property was successfully retrieved to 'propValue',
    eInvalidInput if 'pField' does not contain data defining the requested
    property, eWrongDatabase if the sheet set (OdSmDatabase or OdSmSheetSet)
    could not be found using 'pDb'.
  */
  virtual OdResult getSheetSetProperty(OdString& propValue, OdDbField* pField, OdDbDatabase* pDb) = 0;

  /** \details
    Parses the property string of the ObjProp field code and retrieves object ID
    and property type.

    \param prop      [in] Property string of the format: "Object(%<\_ObjId XXX>%).PropName".
    \param pDb       [in] Database.
    \param objId    [out] Object ID.
    \param propName [out] Property name.

    \returns True if the operation is successful, false otherwise.
  */
  virtual bool parseObjProp(const OdString& prop, OdDbDatabase* pDb, OdDbObjectId& objId, OdString& propName) = 0;

  /** \details
    Retrieves the object property value.

    \param propName [in] Property name.
    \param paramId  [in] Optional dynamic block parameter ID (0 by default).
    \param objId    [in] Object ID.
    \param result  [out] Resulting property value.

    \returns True if the operation is successful, false otherwise.
  */
  virtual bool getObjPropValue(const OdString& propName, OdInt32 paramId, const OdDbObjectId& objId, OdFdFieldResult& result) = 0;

  /** \details
    Retrieves the current database of the host application. The purpose of the
    function is to allow using the current database's properties when processing
    NDBRO fields.

    \returns Pointer to the current OdDbDatabase object.
  */
  virtual OdDbDatabase* getCurrentDb() = 0;

  /** \details
    Returns the name of the file associated with this database object.

    \param pDb [in] Database.
    \returns Name of the file associated with this database object.
    \remarks
    Differs from the current behavior of OdDbDatabase::getFilename(), which
    returns the autobackup filename, because OdDbDatabase::writeFile() always
    sets m_currentFileName.
  */
  virtual OdString getFileName(OdDbDatabase* pDb) = 0;

  /** \details
    Retrieves the lisp variable value.

    \param lispVar [in] Lisp variable name.
    \param fValue [out] Field value where the result is stored.
  */
  virtual void getLispVariable(const OdString& lispVar, OdFieldValue& fValue) = 0;

  /** \details
    Returns the OdDbTable entity ID which is supposed to be the owner of the
    field object. Intended to be used during adding or modifying a formula field
    in one of table cells, while the field can be a non-database-resident.

    \param field [in] Pointer to the field object.
    \returns OdDbTable entity ID.
  */
  virtual OdDbObjectId getOwnerTable(const OdDbField* field) = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for
  OdFdFieldEnginePE object pointers.
*/
typedef OdSmartPtr<OdFdFieldEnginePE> OdFdFieldEnginePEPtr;

/** \details
  Evaluates fields in database.
  \param pDb               [in] Pointer to the database for which fields are to be evaluated.
  \param nContext          [in] Input context in which the field is evaluated, which can be a
                                flag, predefined by the OdDbField::EvalContext enumeration,
                                or a user-defined context flag; this context flag is passed
                                to the evaluator.
  \param objIds            [in] Input array of object IDs of objects, whose fields are to be
                                evaluated. If this is not NULL only, fields will be evaluated
                                only for these objects.
  \param pFieldsToEvaluate [in] Input array of object IDs of fields to be evaluated. If this
                                parameter is not NULL, only these fields in the specified objects
                                will be evaluated. If this parameter is NULL, all the fields
                                in the specified objects will be evaluated.
  \param pszEvaluatorId    [in] Input evaluator ID. If this parameter is not NULL, only the fields
                                matching this evaluator ID will be evaluated. If this parameter is
                                NULL, all the fields in the specified objects will be evaluated.
  \param nEvalFlag         [in] One or more OdFd::EvalFields option flags.
  \param pNumFound        [out] Output pointer to store the number of fields found; can be NULL
                                if this information is not required.
  \param pNumEvaluated    [out] Output pointer to store the number of fields evaluated; can be
                                NULL if this information is not required.
  \returns eOk if the operation is successful or an appropriate error code
  otherwise.
*/
DBENT_EXPORT OdResult oddbEvaluateFields(
  OdDbDatabase* pDb,
  int nContext,
  const OdDbObjectIdArray* objIds = 0,
  const OdDbObjectIdArray* pFieldsToEvaluate = 0,
  const OdString& pszEvaluatorId = OdString(),
  OdFd::EvalFields nEvalFlag = OdFd::kEvalRecursive,
  int* pNumFound = 0,
  int* pNumEvaluated = 0);

/** \details
  Splits a string into substrings: fields and the text separating them. The
  order remains the same.

  \param sString      [in] String to split.
  \param aSubStrings [out] Array of strings, consisting of individual fields and portions of plain text.
*/
DBENT_EXPORT void odfdGetSubStrings(
  const OdString& sString,
  OdStringArray& aSubStrings);

/** \details
  Parses a string containing a field with AcVar, separating the variable name
  and formatting.

  \param sString  [in] String to parse.
  \param sName   [out] Name of the AcVar.
  \param sFormat [out] Format of the AcVar.
  \returns True if the specified string contains AcVar, false otherwise.
*/
DBENT_EXPORT bool odfdGetAcVarData(
  const OdString& sString,
  OdString& sName,
  OdString& sFormat);


#include "TD_PackPop.h"

#endif //_FdField_h_Included_
