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
#ifndef _OD_RANDOM_H_
#define _OD_RANDOM_H_

#include "OdaCommon.h"
#include "OdArray.h"

/** \details
    <group Other_Classes>
	Base class for pseudorandom data generators.
	\remark Do NOT use it directly, use OdRandom instead.
*/
class FIRSTDLL_EXPORT OdRandomGen {
public:
  virtual ~OdRandomGen();
  virtual OdRandomGen *clone() = 0;

  // The future sequence is fully determined by the provided seed.
  // Note: do NOT pass nondeterministic things like time(), otherwise your results won't be reproducible!
  // In most cases you should NOT call it: every generator has a fixed default value.
  virtual void setSeed(OdUInt32 seed) = 0;
  virtual OdUInt32 getSeed() const = 0;

  virtual void getRange(OdUInt32& high) const = 0;
  virtual OdUInt32 generate() = 0;
};


/** \details
    <group Other_Classes>
	Random number generator with support for uniform distributions.
	
 A few important facts:
   1. The state is stored internally (no global state like in rand).
   2. The generated integer numbers are exactly the same on every platform on every run.
   3. Ideally, the same should apply to real numbers: they should be same on every platform on every run.
      Even if they are different, the deviation should be negligible (~ DBL_EPSILON) relative to distribution range.

 Sample usage:
  OdRandom rnd;
  for (int i = 0; i < 1000; i++)
    samples.push_back(rnd.genDouble(0, OdaPI));
*/
class FIRSTDLL_EXPORT OdRandom {
public:
  // If you pass custom random generator, then OdRandom receives ownership over it.
  // Otherwise, a default OdRandomGenMinstd generator will be created.
  explicit OdRandom(OdRandomGen *newGenerator = NULL);

  ~OdRandom();
  OdRandom(const OdRandom &iSrc);
  void operator=(const OdRandom &iSrc);

  OdRandomGen &generator();
  const OdRandomGen &generator() const;

  //note: all integer-valued methods are inclusive, i.e. low <= return <= high
  OdUInt64 genUInt64(OdUInt64 low, OdUInt64 high);
  OdInt64 genInt64(OdInt64 low, OdInt64 high);
  OdUInt32 genUInt(OdUInt32 low, OdUInt32 high);
  int genInt(int low, int high);

  bool genBool();

  //note: real-valued methods are potentially inclusive too, i.e. low <= return <= high
  double genDouble();
  double genDouble(double low, double high);

  template <class T, class A> const T& randomOf(const OdArray<T, A>& iArray) {
    int idx = genInt(1, iArray.size()) - 1;
    return iArray[idx];
  }

private:
  OdUInt64 genAnyInt();

  //owning the generator
  OdRandomGen *m_generator;
  //cache: avoid calling virtual functions too much
  OdUInt32 m_genMax, m_genCalls;
};

//shuffles the given array, so that all possible outcomes are equiprobable
template<class T> void OdRandomShuffle(T *beg, T *end, OdRandom *rnd) {
  ODA_ASSERT(end >= beg);
  ODA_ASSERT(rnd);
  size_t n = end - beg;
  for (size_t i = 0; i < n; i++) {
    size_t pos = (size_t)rnd->genUInt64(0, i);
    { //TODO: is there anything like std::swap ?
      T buff = beg[pos];
      beg[pos] = beg[i];
      beg[i] = buff;
    }
  }
}

//======================== implementations of random number generators ========================

/** \details
    <group Other_Classes>
	Default random data generator: equivalent to std::minstd_rand
*/
class FIRSTDLL_EXPORT OdRandomGenMinstd : public OdRandomGen {
public:
  OdRandomGenMinstd(OdUInt32 seed = 1);
  virtual ~OdRandomGenMinstd();
  virtual OdRandomGenMinstd *clone();

  virtual void setSeed(OdUInt32 seed);
  virtual OdUInt32 getSeed() const;

  virtual void getRange(OdUInt32& high) const;
  virtual OdUInt32 generate();

private:
  OdUInt32 m_state;
};

#endif
