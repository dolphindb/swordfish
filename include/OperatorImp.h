/*
  * OperatorImp.h
 *
 *  Created on: Oct 18, 2013
 *      Author: dzhou
 */

#ifndef OPERATORIMP_H_
#define OPERATORIMP_H_

#include "CoreConcept.h"

namespace OperatorImp{

//functions for other databases
ConstantSP oracleConcat(const ConstantSP& a, const ConstantSP& b);
ConstantSP oracleRowNumber(Heap* heap, vector<ConstantSP>& arguments);

//no argument function
ConstantSP benchmark(const ConstantSP& a, const ConstantSP& b);
ConstantSP now(const ConstantSP& a, const ConstantSP& b);
ConstantSP today(const ConstantSP& a, const ConstantSP& b);
ConstantSP memory(const ConstantSP& a, const ConstantSP& b);
ConstantSP getHomeDir(const ConstantSP& a, const ConstantSP& b);
ConstantSP getNodeAlias(const ConstantSP& a, const ConstantSP& b);
ConstantSP getNodeHost(const ConstantSP& a, const ConstantSP& b);
ConstantSP getNodePort(const ConstantSP& a, const ConstantSP& b);
ConstantSP getOS(const ConstantSP& a, const ConstantSP& b);
ConstantSP getOSBit(const ConstantSP& a, const ConstantSP& b);
ConstantSP version(const ConstantSP& a, const ConstantSP& b);
ConstantSP getEnv(const ConstantSP& a, const ConstantSP& b);
ConstantSP getRequiredAPIVersion(const ConstantSP& a, const ConstantSP& b);
ConstantSP getLicenseExpiration(const ConstantSP& a, const ConstantSP& b);

ConstantSP brief(const ConstantSP& a, const ConstantSP& b);
ConstantSP snippet(const ConstantSP& a, const ConstantSP& b);
ConstantSP syntax(const ConstantSP& a, const ConstantSP& b);
ConstantSP memSize(const ConstantSP& a, const ConstantSP& b);
ConstantSP compress(const ConstantSP& a, const ConstantSP& b);
ConstantSP decompress(const ConstantSP& a, const ConstantSP& b);

ConstantSP func(const ConstantSP& a, const ConstantSP& b);
ConstantSP add(const ConstantSP& a, const ConstantSP& b);
ConstantSP sub(const ConstantSP& a, const ConstantSP& b);
ConstantSP multiply(const ConstantSP& a, const ConstantSP& b);
ConstantSP divide(const ConstantSP& a, const ConstantSP& b);
/** Equivalent to: '/' in Python; and '\' (`ratio`) in DolphinDB. */
ConstantSP dividePython(const ConstantSP& a, const ConstantSP& b);
/** Equivalent to: '//' in Python. */
ConstantSP floorDivide(const ConstantSP& a, const ConstantSP& b);
ConstantSP ratio(const ConstantSP& a, const ConstantSP& b);
ConstantSP mod(const ConstantSP& a, const ConstantSP& b);
ConstantSP modPython(const ConstantSP& a, const ConstantSP& b);
ConstantSP power(const ConstantSP& a, const ConstantSP& b);
ConstantSP minIgnoreNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP maxIgnoreNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP lt(const ConstantSP& a, const ConstantSP& b);
ConstantSP le(const ConstantSP& a, const ConstantSP& b);
ConstantSP gt(const ConstantSP& a, const ConstantSP& b);
ConstantSP ge(const ConstantSP& a, const ConstantSP& b);
ConstantSP between(const ConstantSP& a, const ConstantSP& b);
ConstantSP ltIgnoreNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP leIgnoreNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP gtIgnoreNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP geIgnoreNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP betweenIgnoreNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP ltNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP leNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP gtNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP geNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP betweenNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP ne(const ConstantSP& a, const ConstantSP& b);
ConstantSP equal(const ConstantSP& a, const ConstantSP& b);
ConstantSP logicAnd(const ConstantSP& a, const ConstantSP& b);
ConstantSP logicOr(const ConstantSP& a, const ConstantSP& b);
ConstantSP logicOrIgnoreNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP logicXor(const ConstantSP& a, const ConstantSP& b);
ConstantSP bitAnd(const ConstantSP& a, const ConstantSP& b);
ConstantSP bitOr(const ConstantSP& a, const ConstantSP& b);
ConstantSP bitXor(const ConstantSP& a, const ConstantSP& b);
ConstantSP leftShift(const ConstantSP& a, const ConstantSP& b);
ConstantSP rightShift(const ConstantSP& a, const ConstantSP& b);
ConstantSP symmetricDifference(const ConstantSP& a, const ConstantSP& b);
ConstantSP hashBucket(const ConstantSP& a, const ConstantSP& b);
ConstantSP distance(const ConstantSP& a, const ConstantSP& b);
ConstantSP ifNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP ifValid(const ConstantSP& a, const ConstantSP& b);
ConstantSP notIn(const ConstantSP& a, const ConstantSP& b);
ConstantSP notBetween(const ConstantSP& a, const ConstantSP& b);
ConstantSP notLike(const ConstantSP& a, const ConstantSP& b);
ConstantSP nullIf(const ConstantSP& a, const ConstantSP& b);
ConstantSP neAny(const ConstantSP& a, const ConstantSP& b);
ConstantSP eqAll(const ConstantSP& a, const ConstantSP& b);

ConstantSP at(const ConstantSP& a, const ConstantSP& b);
ConstantSP at(Heap* heap, const ConstantSP& a, const ConstantSP& b);
ConstantSP eachAt(const ConstantSP& a, const ConstantSP& b);
ConstantSP eachAt(Heap* heap, const ConstantSP& a, const ConstantSP& b);
ConstantSP mask(const ConstantSP& a, const ConstantSP& b);
ConstantSP member(const ConstantSP& a, const ConstantSP& b);
ConstantSP rand(const ConstantSP& a,const ConstantSP& b);
ConstantSP polynomial(const ConstantSP& a,const ConstantSP& b);
ConstantSP take(const ConstantSP& a, const ConstantSP& b);
ConstantSP stretch(const ConstantSP& a, const ConstantSP& b);
ConstantSP drop(const ConstantSP& a, const ConstantSP& b);
ConstantSP til(const ConstantSP& a, const ConstantSP& b);
ConstantSP seq(const ConstantSP& a, const ConstantSP& b);
ConstantSP pair(const ConstantSP& a, const ConstantSP& b);
ConstantSP in(const ConstantSP& a, const ConstantSP& b);
ConstantSP find(const ConstantSP& a, const ConstantSP& b);
ConstantSP binsrch(const ConstantSP& a, const ConstantSP& b);
ConstantSP asof(const ConstantSP& a, const ConstantSP& b);
ConstantSP cut(const ConstantSP& a, const ConstantSP& b);
ConstantSP cast(const ConstantSP& a, const ConstantSP& b);
ConstantSP reshape(const ConstantSP& a, const ConstantSP& b);
ConstantSP join(const ConstantSP& a, const ConstantSP& b);
ConstantSP concatMatrix(const ConstantSP& a, const ConstantSP& b);
ConstantSP subarray(const ConstantSP& a, const ConstantSP& b);
ConstantSP subtuple(const ConstantSP& a, const ConstantSP& b);
ConstantSP arrayVector(const ConstantSP& a, const ConstantSP& b);
ConstantSP slicedTable(const ConstantSP& a, const ConstantSP& b);
ConstantSP head(const ConstantSP& a, const ConstantSP& b);
ConstantSP tail(const ConstantSP& a, const ConstantSP& b);
ConstantSP indexedSeries(const ConstantSP& a, const ConstantSP& b);
ConstantSP enlist(const ConstantSP& a, const ConstantSP& b);
ConstantSP lowerBound(const ConstantSP& a, const ConstantSP& b);

ConstantSP linearTimeTrend(const ConstantSP& a, const ConstantSP& b);
ConstantSP kama(const ConstantSP& a, const ConstantSP& b);
ConstantSP wilder(const ConstantSP& a, const ConstantSP& b);
ConstantSP ema(Heap *pHeap, vector<ConstantSP>& arguments);
ConstantSP sma(const ConstantSP& a, const ConstantSP& b);
ConstantSP wma(const ConstantSP& a, const ConstantSP& b);
ConstantSP dema(const ConstantSP& a, const ConstantSP& b);
ConstantSP tema(const ConstantSP& a, const ConstantSP& b);
ConstantSP trima(const ConstantSP& a, const ConstantSP& b);
ConstantSP t3(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP gema(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ma(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP sessionWindow(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP talibNull(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mcount(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mfirst(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mfirstNot(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mifirstNot(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mlast(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mlastNot(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP milastNot(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mmed(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mavg(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mmin(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mmax(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mimin(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mimax(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP miminLast(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP mimaxLast(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP msum(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP msum2(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mprod(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mstd(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mvar(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mstdp(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mvarp(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mskew(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mkurtosis(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mrank(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mcorr(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mcovar(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mbeta(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mwsum(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mwavg(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mslr(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mpercentile(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mmse(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mmad(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP matImin(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP matImax(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mmaxPositiveStreak(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmcount(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmfirst(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmlast(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmsum(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmsum2(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmmed(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmpercentile(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmavg(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmstd(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmvar(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmstdp(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmvarp(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmskew(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmkurtosis(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmprod(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmmin(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmmax(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmrank(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmcorr(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmcovar(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmbeta(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmwsum(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmwavg(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmove(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmatImin(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmatImax(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ftmsum(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ftmmax(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ftmmin(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ftmprod(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ftmlast(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ftmwavg(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cumskewTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cumkurtosisTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cumsumTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cumavgTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cumvarTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cumvarpTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cumstdTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cumstdpTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cumcorrTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cumbetaTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cumcovarTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cumwsumTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mskewTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mkurtosisTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP msumTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mavgTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mvarTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mvarpTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mstdTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mstdpTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mcorrTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mbetaTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mcovarTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mwsumTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mpercentileTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmskewTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmkurtosisTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmsumTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmavgTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmvarTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmvarpTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmstdTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmstdpTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmcorrTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmbetaTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmcovarTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmwsumTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cumTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmovingTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP movingTopN(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP movingTopNIndex(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP movingWindowIndex(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP movingWindowData(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmovingWindowIndex(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmovingWindowData(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ewmMean(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP ewmVar(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP ewmStd(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP ewmCorr(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP ewmCov(Heap* heap, vector<ConstantSP>& arguments);

//vector manipulation
ConstantSP rowNo(const ConstantSP& a, const ConstantSP& b);
ConstantSP move(const ConstantSP& a, const ConstantSP& b);
ConstantSP next(const ConstantSP& a, const ConstantSP& b);
ConstantSP prev(const ConstantSP& a, const ConstantSP& b);
ConstantSP lead(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP lag(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP nextState(const ConstantSP& a, const ConstantSP& b);
ConstantSP prevState(const ConstantSP& a, const ConstantSP& b);
ConstantSP reverse(const ConstantSP& a, const ConstantSP& b);
ConstantSP shuffle(const ConstantSP& a, const ConstantSP& b);
ConstantSP isortUnique(const ConstantSP& a, const ConstantSP& b);
ConstantSP isort(const ConstantSP& a, const ConstantSP& b);
ConstantSP sort(const ConstantSP& a, const ConstantSP& b);
ConstantSP distinct(const ConstantSP& a, const ConstantSP& b);
ConstantSP groups(const ConstantSP& a, const ConstantSP& b);
ConstantSP keys(const ConstantSP& a, const ConstantSP& b);
ConstantSP values(const ConstantSP& a, const ConstantSP& b);
ConstantSP iterator(const ConstantSP& a, const ConstantSP& b);
ConstantSP row(const ConstantSP& a, const ConstantSP& b);
ConstantSP column(const ConstantSP& a, const ConstantSP& b);
ConstantSP columnNames(const ConstantSP& a, const ConstantSP& b);
ConstantSP rowNames(const ConstantSP& a, const ConstantSP& b);

//aggregate function
ConstantSP isOrderedDict(const ConstantSP& a, const ConstantSP& b);
ConstantSP isIndexedMatrix(const ConstantSP& a, const ConstantSP& b);
ConstantSP isIndexedSeries(const ConstantSP& a, const ConstantSP& b);
ConstantSP isColumnarTuple(const ConstantSP& a, const ConstantSP& b);
ConstantSP isVoid(const ConstantSP& a, const ConstantSP& b);
ConstantSP isNothing(const ConstantSP& a, const ConstantSP& b);
ConstantSP type(const ConstantSP& a, const ConstantSP& b);
ConstantSP typestr(const ConstantSP& a, const ConstantSP& b);
ConstantSP category(const ConstantSP& a, const ConstantSP& b);
ConstantSP form(const ConstantSP& a, const ConstantSP& b);
ConstantSP hasNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP size(const ConstantSP& a, const ConstantSP& b);
ConstantSP capacity(const ConstantSP& a, const ConstantSP& b);
ConstantSP count(const ConstantSP& a, const ConstantSP& b);
ConstantSP countNanInf(const ConstantSP& a, const ConstantSP& b);
ConstantSP contextCount(const ConstantSP& a, const ConstantSP& b);
ConstantSP rows(const ConstantSP& a, const ConstantSP& b);
ConstantSP columns(const ConstantSP& a, const ConstantSP& b);
ConstantSP shape(const ConstantSP& a, const ConstantSP& b);
ConstantSP tupleSum(const ConstantSP& a, const ConstantSP& b);
ConstantSP sum(const ConstantSP& a, const ConstantSP& b);
ConstantSP contextSum(const ConstantSP& a, const ConstantSP& b);
ConstantSP sum2(const ConstantSP& a, const ConstantSP& b);
ConstantSP sum3(const ConstantSP& a, const ConstantSP& b);
ConstantSP sum4(const ConstantSP& a, const ConstantSP& b);
ConstantSP contextSum2(const ConstantSP& a, const ConstantSP& b);
ConstantSP mad(const ConstantSP& a, const ConstantSP& b);
ConstantSP sem(const ConstantSP& a, const ConstantSP& b);
ConstantSP avg(const ConstantSP& a, const ConstantSP& b);
ConstantSP var(const ConstantSP& a, const ConstantSP& b);
ConstantSP std(const ConstantSP& a, const ConstantSP& b);
ConstantSP varp(const ConstantSP& a, const ConstantSP& b);
ConstantSP stdp(const ConstantSP& a, const ConstantSP& b);
ConstantSP skew(const ConstantSP& a, const ConstantSP& b);
ConstantSP kurtosis(const ConstantSP& a, const ConstantSP& b);
ConstantSP prod(const ConstantSP& a, const ConstantSP& b);
ConstantSP max(const ConstantSP& a, const ConstantSP& b);
ConstantSP min(const ConstantSP& a, const ConstantSP& b);
ConstantSP imax(const ConstantSP& a, const ConstantSP& b);
ConstantSP imin(const ConstantSP& a, const ConstantSP& b);
ConstantSP imaxLast(const ConstantSP &a, const ConstantSP &b);
ConstantSP iminLast(const ConstantSP &a, const ConstantSP &b);
ConstantSP first(const ConstantSP& a, const ConstantSP& b);
ConstantSP last(const ConstantSP& a, const ConstantSP& b);
ConstantSP lastNot(const ConstantSP& a, const ConstantSP& b);
ConstantSP firstNot(const ConstantSP& a, const ConstantSP& b);
ConstantSP ilastNot(const ConstantSP& a, const ConstantSP& b);
ConstantSP ifirstNot(const ConstantSP& a, const ConstantSP& b);
ConstantSP med(const ConstantSP& a, const ConstantSP& b);
ConstantSP mode(const ConstantSP& a, const ConstantSP& b);
ConstantSP stat(const ConstantSP& a, const ConstantSP& b);
ConstantSP maxPositiveStreak(const ConstantSP& a, const ConstantSP& b);
ConstantSP allTrue(const ConstantSP& a, const ConstantSP& b);
ConstantSP anyTrue(const ConstantSP& a, const ConstantSP& b);
ConstantSP oddTrue(const ConstantSP& a, const ConstantSP& b);

ConstantSP isSorted(const ConstantSP& a, const ConstantSP& b);
ConstantSP atIMax(const ConstantSP& a, const ConstantSP& b);
ConstantSP atIMin(const ConstantSP& a, const ConstantSP& b);
ConstantSP wavg(const ConstantSP& a, const ConstantSP& b);
ConstantSP wsum(const ConstantSP& a, const ConstantSP& b);
ConstantSP dot(const ConstantSP& a, const ConstantSP& b);
ConstantSP euclidean(const ConstantSP& a, const ConstantSP& b);
ConstantSP jaccard(const ConstantSP& a, const ConstantSP& b);
ConstantSP tanimoto(const ConstantSP& a, const ConstantSP& b);
ConstantSP correlation(const ConstantSP& a, const ConstantSP& b);
ConstantSP spearmanr(const ConstantSP& a, const ConstantSP& b);
ConstantSP kendall(const ConstantSP& a, const ConstantSP& b);
ConstantSP mutualInformation(const ConstantSP& a, const ConstantSP& b);
ConstantSP covariance(const ConstantSP& a, const ConstantSP& b);
ConstantSP beta(const ConstantSP& a, const ConstantSP& b);
ConstantSP searchK(const ConstantSP& a, const ConstantSP& b);
ConstantSP difference(const ConstantSP& a, const ConstantSP& b);
ConstantSP crossStat(const ConstantSP& a, const ConstantSP& b);
ConstantSP isMonotonic(const ConstantSP& a, const ConstantSP& b);
ConstantSP isMonotonicIncreasing(const ConstantSP& a, const ConstantSP& b);
ConstantSP isMonotonicDecreasing(const ConstantSP& a, const ConstantSP& b);

ConstantSP eye(const ConstantSP& a, const ConstantSP& b);
ConstantSP diag(const ConstantSP& a, const ConstantSP& b);
ConstantSP naiveMulti(const ConstantSP& a, const ConstantSP& b);
ConstantSP strassenMulti(const ConstantSP& a, const ConstantSP& b);
ConstantSP transpose(const ConstantSP& a, const ConstantSP& b);
ConstantSP inverse(const ConstantSP& a, const ConstantSP& b);
ConstantSP pinverse(const ConstantSP& a, const ConstantSP& b);
ConstantSP det(const ConstantSP& a, const ConstantSP& b);
ConstantSP solve(const ConstantSP& a, const ConstantSP& b);
ConstantSP cholesky(const ConstantSP& a, const ConstantSP& b);
ConstantSP lu(const ConstantSP& a, const ConstantSP& b);
ConstantSP schur(const ConstantSP& a, const ConstantSP& b);
ConstantSP triu(const ConstantSP& a, const ConstantSP& b);
ConstantSP tril(const ConstantSP& a, const ConstantSP& b);

ConstantSP writeBytes(const ConstantSP& a, const ConstantSP& b);
ConstantSP readBytes(const ConstantSP& a, const ConstantSP& b);
ConstantSP readLine(const ConstantSP& a, const ConstantSP& b);
ConstantSP readLines(const ConstantSP& a, const ConstantSP& b);
ConstantSP toJson(const ConstantSP& a, const ConstantSP& b);
ConstantSP toStdJson(const ConstantSP& a, const ConstantSP& b);
ConstantSP fromJson(const ConstantSP& a, const ConstantSP& b);
ConstantSP fromStdJson(const ConstantSP& a, const ConstantSP& b);
ConstantSP crc32(const ConstantSP& a, const ConstantSP& b);
ConstantSP md5(const ConstantSP& a, const ConstantSP& b);
ConstantSP wideTable(const ConstantSP& a, const ConstantSP& b);

//unary temporal functions
ConstantSP year(const ConstantSP& a, const ConstantSP& b);
ConstantSP month(const ConstantSP& a, const ConstantSP& b);
ConstantSP date(const ConstantSP& a, const ConstantSP& b);
ConstantSP hour(const ConstantSP& a, const ConstantSP& b);
ConstantSP minute(const ConstantSP& a, const ConstantSP& b);
ConstantSP second(const ConstantSP& a, const ConstantSP& b);
ConstantSP time(const ConstantSP& a, const ConstantSP& b);
ConstantSP datetime(const ConstantSP& a, const ConstantSP& b);
ConstantSP datehour(const ConstantSP& a, const ConstantSP& b);
ConstantSP timestamp(const ConstantSP& a, const ConstantSP& b);
ConstantSP nanotime(const ConstantSP& a, const ConstantSP& b);
ConstantSP nanotimestamp(const ConstantSP& a, const ConstantSP& b);
ConstantSP weekday(const ConstantSP& a, const ConstantSP& b);
ConstantSP dayOfWeek(const ConstantSP& a, const ConstantSP& b);
ConstantSP localtime(const ConstantSP& a, const ConstantSP& b);
ConstantSP gmtime(const ConstantSP& a, const ConstantSP& b);
ConstantSP concatDateTime(const ConstantSP& a, const ConstantSP& b);
ConstantSP dayOfYear(const ConstantSP& a, const ConstantSP& b);
ConstantSP dayOfMonth(const ConstantSP& a, const ConstantSP& b);
ConstantSP quarterOfYear(const ConstantSP& a, const ConstantSP& b);
ConstantSP monthOfYear(const ConstantSP& a, const ConstantSP& b);
ConstantSP weekOfYear(const ConstantSP& a, const ConstantSP& b);
ConstantSP hourOfDay(const ConstantSP& a, const ConstantSP& b);
ConstantSP minuteOfHour(const ConstantSP& a, const ConstantSP& b);
ConstantSP secondOfMinute(const ConstantSP& a, const ConstantSP& b);
ConstantSP millisecond(const ConstantSP& a, const ConstantSP& b);
ConstantSP microsecond(const ConstantSP& a, const ConstantSP& b);
ConstantSP nanosecond(const ConstantSP& a, const ConstantSP& b);
ConstantSP isMonthStart(const ConstantSP& a, const ConstantSP& b);
ConstantSP isMonthEnd(const ConstantSP& a, const ConstantSP& b);
ConstantSP isQuarterStart(const ConstantSP& a, const ConstantSP& b);
ConstantSP isQuarterEnd(const ConstantSP& a, const ConstantSP& b);
ConstantSP isYearStart(const ConstantSP& a, const ConstantSP& b);
ConstantSP isYearEnd(const ConstantSP& a, const ConstantSP& b);
ConstantSP isLeapYear(const ConstantSP& a, const ConstantSP& b);
ConstantSP daysInMonth(const ConstantSP& a, const ConstantSP& b);

//data type conversion
ConstantSP decodeShortGenomeSeq(const ConstantSP& a, const ConstantSP& b);
ConstantSP encodeShortGenomeSeq(const ConstantSP& a, const ConstantSP& b);
ConstantSP genShortGenomeSeq(const ConstantSP& a, const ConstantSP& b);
ConstantSP symbolCode(const ConstantSP& a, const ConstantSP& b);
ConstantSP asBool(const ConstantSP& a, const ConstantSP& b);
ConstantSP asChar(const ConstantSP& a, const ConstantSP& b);
ConstantSP asShort(const ConstantSP& a, const ConstantSP& b);
ConstantSP asInt(const ConstantSP& a, const ConstantSP& b);
ConstantSP asLong(const ConstantSP& a, const ConstantSP& b);
ConstantSP asFloat(const ConstantSP& a, const ConstantSP& b);
ConstantSP asDouble(const ConstantSP& a, const ConstantSP& b);
ConstantSP asSymbol(const ConstantSP& a, const ConstantSP& b);
ConstantSP asString(const ConstantSP& a, const ConstantSP& b);
ConstantSP asBlob(const ConstantSP& a, const ConstantSP& b);
ConstantSP asUuid(const ConstantSP& a, const ConstantSP& b);
ConstantSP asIPAddr(const ConstantSP& a, const ConstantSP& b);
ConstantSP asInt128(const ConstantSP& a, const ConstantSP& b);
ConstantSP asComplex(const ConstantSP& a, const ConstantSP& b);
ConstantSP asPoint(const ConstantSP& a, const ConstantSP& b);
ConstantSP asDuration(const ConstantSP& a, const ConstantSP& b);
ConstantSP makeDuration(const ConstantSP& a, const ConstantSP& b);
ConstantSP highDouble(const ConstantSP& a, const ConstantSP& b);
ConstantSP lowDouble(const ConstantSP& a, const ConstantSP& b);
ConstantSP highLong(const ConstantSP& a, const ConstantSP& b);
ConstantSP lowLong(const ConstantSP& a, const ConstantSP& b);

/**
 * @brief cast other object to Decimal
 * 
 * @param a object to be casted
 * @param b scale
 */
ConstantSP asDecimal32(const ConstantSP& a, const ConstantSP& b);
ConstantSP asDecimal64(const ConstantSP& a, const ConstantSP& b);
ConstantSP asDecimal128(const ConstantSP& a, const ConstantSP& b);

ConstantSP decimalMultiply(Heap *heap, vector<ConstantSP> &arguments);

//unary math functions
ConstantSP signum(const ConstantSP& a, const ConstantSP& b);
ConstantSP signbit(const ConstantSP& a, const ConstantSP& b);
ConstantSP nullFlag(const ConstantSP& a, const ConstantSP& b);
ConstantSP isNull(const ConstantSP& a, const ConstantSP& b);
ConstantSP isValid(const ConstantSP& a, const ConstantSP& b);
ConstantSP isNanInf(const ConstantSP& a, const ConstantSP& b);
ConstantSP nullFill(const ConstantSP& a, const ConstantSP& b);
ConstantSP nanInfFill(const ConstantSP& a, const ConstantSP& b);
ConstantSP where(const ConstantSP& a, const ConstantSP& b);
ConstantSP logicNot(const ConstantSP& a, const ConstantSP& b);
ConstantSP bitNot(const ConstantSP& a, const ConstantSP& b);
ConstantSP round(const ConstantSP& a, const ConstantSP& b);
ConstantSP roundPandas(const ConstantSP& a, const ConstantSP& b);
ConstantSP decimalFormat(const ConstantSP& a, const ConstantSP& b);
ConstantSP temporalFormat(const ConstantSP& a, const ConstantSP& b);
ConstantSP format(const ConstantSP& a, const ConstantSP& b);
ConstantSP temporalParse(const ConstantSP& a, const ConstantSP& b);
ConstantSP ceil(const ConstantSP& a, const ConstantSP& b);
ConstantSP floor(const ConstantSP& a, const ConstantSP& b);
ConstantSP neg(const ConstantSP& a, const ConstantSP& b);
ConstantSP abs(const ConstantSP& a, const ConstantSP& b);
ConstantSP reciprocal(const ConstantSP& a, const ConstantSP& b);
ConstantSP sin(const ConstantSP& a, const ConstantSP& b);
ConstantSP cos(const ConstantSP& a, const ConstantSP& b);
ConstantSP tan(const ConstantSP& a, const ConstantSP& b);
ConstantSP asin(const ConstantSP& a, const ConstantSP& b);
ConstantSP acos(const ConstantSP& a, const ConstantSP& b);
ConstantSP atan(const ConstantSP& a, const ConstantSP& b);
ConstantSP sinh(const ConstantSP& a, const ConstantSP& b);
ConstantSP cosh(const ConstantSP& a, const ConstantSP& b);
ConstantSP tanh(const ConstantSP& a, const ConstantSP& b);
ConstantSP asinh(const ConstantSP& a, const ConstantSP& b);
ConstantSP acosh(const ConstantSP& a, const ConstantSP& b);
ConstantSP atanh(const ConstantSP& a, const ConstantSP& b);
ConstantSP deg2rad(const ConstantSP& a, const ConstantSP& b);
ConstantSP rad2deg(const ConstantSP& a, const ConstantSP& b);
ConstantSP sqrt(const ConstantSP& a, const ConstantSP& b);
ConstantSP exp(const ConstantSP& a, const ConstantSP& b);
ConstantSP log(const ConstantSP& a, const ConstantSP& b);
ConstantSP exp2(const ConstantSP& a, const ConstantSP& b);
ConstantSP expm1(const ConstantSP& a, const ConstantSP& b);
ConstantSP log2(const ConstantSP& a, const ConstantSP& b);
ConstantSP log10(const ConstantSP& a, const ConstantSP& b);
ConstantSP log1p(const ConstantSP& a, const ConstantSP& b);
ConstantSP cbrt(const ConstantSP& a, const ConstantSP& b);
ConstantSP square(const ConstantSP& a, const ConstantSP& b);
ConstantSP flatten(const ConstantSP& a, const ConstantSP& b);
ConstantSP copy(const ConstantSP& a, const ConstantSP& b);
ConstantSP asis(const ConstantSP& a, const ConstantSP& b);

ConstantSP cumPositiveStreak(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumsum(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumsum2(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumsum3(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumsum4(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumprod(const ConstantSP& a, const ConstantSP& b);
ConstantSP cummin(const ConstantSP& a, const ConstantSP& b);
ConstantSP cummax(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumcount(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumavg(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumstd(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumvar(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumstdp(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumvarp(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumpercentile(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP cumrank(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP cumdenseRank(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP cummed(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumcorr(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumcovar(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumbeta(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumwsum(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumwavg(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumfirstNot(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumlastNot(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumifirstNot(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumilastNot(const ConstantSP& a, const ConstantSP& b);
ConstantSP cumnunique(const ConstantSP& a, const ConstantSP& b);
ConstantSP dynamicGroupCumsum(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP dynamicGroupCumcount(Heap* heap, vector<ConstantSP>& arguments);

ConstantSP deltas(const ConstantSP& a, const ConstantSP& b);
ConstantSP ratios(const ConstantSP& a, const ConstantSP& b);
ConstantSP bar(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP volumeBar(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP zscore(const ConstantSP& a, const ConstantSP& b);
ConstantSP demean(const ConstantSP& a, const ConstantSP& b);
ConstantSP percentChange(const ConstantSP& a, const ConstantSP& b);
ConstantSP segment(const ConstantSP& a, const ConstantSP& b);
ConstantSP topRange(const ConstantSP& a, const ConstantSP& b);
ConstantSP lowRange(const ConstantSP& a, const ConstantSP& b);
ConstantSP isPeak(const ConstantSP& a, const ConstantSP& b);
ConstantSP isValley(const ConstantSP& a, const ConstantSP& b);
ConstantSP sumbars(const ConstantSP& a, const ConstantSP& b);
ConstantSP valueChanged(const ConstantSP& a, const ConstantSP& b);

//string functions
ConstantSP isUpper(const ConstantSP& a, const ConstantSP& b);
ConstantSP isLower(const ConstantSP& a, const ConstantSP& b);
ConstantSP isTitle(const ConstantSP& a, const ConstantSP& b);
ConstantSP isSpace(const ConstantSP& a, const ConstantSP& b);
ConstantSP isAlpha(const ConstantSP& a, const ConstantSP& b);
ConstantSP isNumeric(const ConstantSP& a, const ConstantSP& b);
ConstantSP isAlNum(const ConstantSP& a, const ConstantSP& b);
ConstantSP isDigit(const ConstantSP& a, const ConstantSP& b);
ConstantSP capitalize(const ConstantSP& a, const ConstantSP& b);
ConstantSP title(const ConstantSP& a, const ConstantSP& b);
ConstantSP swapCase(const ConstantSP& a, const ConstantSP& b);
ConstantSP like(const ConstantSP& a, const ConstantSP& b);
ConstantSP ilike(const ConstantSP& a, const ConstantSP& b);
ConstantSP trim(const ConstantSP& a, const ConstantSP& b);
ConstantSP strip(const ConstantSP& a, const ConstantSP& b);
ConstantSP strlen(const ConstantSP& a, const ConstantSP& b);
ConstantSP strlenu(const ConstantSP& a, const ConstantSP& b);
ConstantSP wc(const ConstantSP& a, const ConstantSP& b);
ConstantSP upper(const ConstantSP& a, const ConstantSP& b);
ConstantSP lower(const ConstantSP& a, const ConstantSP& b);
ConstantSP repeat(const ConstantSP& a, const ConstantSP& b);
ConstantSP left(const ConstantSP& a, const ConstantSP& b);
ConstantSP right(const ConstantSP& a, const ConstantSP& b);
ConstantSP ltrim(const ConstantSP& a, const ConstantSP& b);
ConstantSP rtrim(const ConstantSP& a, const ConstantSP& b);
ConstantSP startsWith(const ConstantSP& a, const ConstantSP& b);
ConstantSP endsWith(const ConstantSP& a, const ConstantSP& b);
ConstantSP strpos(const ConstantSP& a, const ConstantSP& b);
ConstantSP charAt(const ConstantSP& a, const ConstantSP& b);
ConstantSP split(const ConstantSP& a, const ConstantSP& b);
ConstantSP concat(const ConstantSP& a, const ConstantSP& b);
ConstantSP hex(const ConstantSP& a, const ConstantSP& b);
ConstantSP binary(const ConstantSP& a, const ConstantSP& b);
ConstantSP initcap(const ConstantSP& a, const ConstantSP& b);
ConstantSP base64Encode(const ConstantSP& a, const ConstantSP& b);
ConstantSP base64Decode(const ConstantSP& a, const ConstantSP& b);
ConstantSP strReplace(Heap * heap,vector<ConstantSP>& arguments);
ConstantSP substr(Heap * heap,vector<ConstantSP>& arguments);
ConstantSP substru(Heap * heap,vector<ConstantSP>& arguments);
ConstantSP lpad(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP rpad(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP regexReplace(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP regexFind(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP regexCount(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP regexFindStr(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP convertEncode(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP toUTF8(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP fromUTF8(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP makeKey(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP makeSortedKey(Heap* heap, vector<ConstantSP>& arguments);

//adverb functions
ConstantSP each(const ConstantSP& a, const ConstantSP& b, const string& optrName, OptrFunc optr, FastFunc fastImp, bool consistent);
ConstantSP loop(const ConstantSP& a, const ConstantSP& b, const string& optrName, OptrFunc optr, FastFunc fastImp, bool consistent);
ConstantSP eachPre(const ConstantSP& a, const ConstantSP& b, const string& optrName, OptrFunc optr, FastFunc fastImp, bool consistent);
ConstantSP eachPost(const ConstantSP& a, const ConstantSP& b, const string& optrName, OptrFunc optr, FastFunc fastImp, bool consistent);
ConstantSP eachRight(const ConstantSP& a, const ConstantSP& b, const string& optrName, OptrFunc optr, FastFunc fastImp, bool consistent);
ConstantSP eachLeft(const ConstantSP& a, const ConstantSP& b, const string& optrName, OptrFunc optr, FastFunc fastImp, bool consistent);
ConstantSP cross(const ConstantSP& a, const ConstantSP& b, const string& optrName, OptrFunc optr, FastFunc fastImp, bool consistent);
ConstantSP reduce(const ConstantSP& a, const ConstantSP& b, const string& optrName, OptrFunc optr, FastFunc fastImp, bool consistent);
ConstantSP accumulate(const ConstantSP& a, const ConstantSP& b, const string& optrName, OptrFunc optr, FastFunc fastImp, bool consistent);
ConstantSP byRow(const ConstantSP& a, const ConstantSP& b, const string& optrName, OptrFunc optr, FastFunc fastImp, bool consistent);
ConstantSP byColumn(const ConstantSP& a, const ConstantSP& b, const string& optrName, OptrFunc optr, FastFunc fastImp, bool consistent);

ConstantSP eachFD(Heap* heap, const ConstantSP& a, const ConstantSP& b,const FunctionDefSP& optr, bool consistent);
ConstantSP loopFD(Heap* heap, const ConstantSP& a, const ConstantSP& b,const FunctionDefSP& optr, bool consistent);
ConstantSP eachPreFD(Heap* heap, const ConstantSP& a, const ConstantSP& b,const FunctionDefSP& optr, bool consistent);
ConstantSP eachPostFD(Heap* heap, const ConstantSP& a, const ConstantSP& b,const FunctionDefSP& optr, bool consistent);
ConstantSP eachRightFD(Heap* heap, const ConstantSP& a, const ConstantSP& b,const FunctionDefSP& optr, bool consistent);
ConstantSP eachLeftFD(Heap* heap, const ConstantSP& a, const ConstantSP& b,const FunctionDefSP& optr, bool consistent);
ConstantSP crossFD(Heap* heap, const ConstantSP& a, const ConstantSP& b,const FunctionDefSP& optr, bool consistent);
ConstantSP reduceFD(Heap* heap, const ConstantSP& a, const ConstantSP& b,const FunctionDefSP& optr, bool consistent);
ConstantSP accumulateFD(Heap* heap, const ConstantSP& a, const ConstantSP& b,const FunctionDefSP& optr, bool consistent);
ConstantSP groupFD(Heap* heap, const ConstantSP& a, const ConstantSP& b,const FunctionDefSP& optr, bool consistent);
ConstantSP contextFD(Heap* heap, const ConstantSP& a, const ConstantSP& b,const FunctionDefSP& optr, bool consistent);
ConstantSP byRowFD(Heap* heap, const ConstantSP& a, const ConstantSP& b,const FunctionDefSP& optr, bool consistent);
ConstantSP byColumnFD(Heap* heap, const ConstantSP& a, const ConstantSP& b,const FunctionDefSP& optr, bool consistent);

//system functions
ConstantSP quantile(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP quantileSeries(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP percentile(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP percentileRank(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP firstHit(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ifirstHit(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP geoWithin(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rdp(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP zigzag(Heap *heap, vector<ConstantSP> &arguments);

ConstantSP sqlCol(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP sqlColAlias(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP sqlTuple(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP expression(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP binaryExpr(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP unifiedExpr(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP makeCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP makeUnifiedCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP makeObjCall(Heap*, vector<ConstantSP>& arguments);
ConstantSP makeUnifiedObjCall(Heap*, vector<ConstantSP>& arguments);
ConstantSP isInstanceOf(Heap*, vector<ConstantSP>& arguments);
ConstantSP setAttr(Heap*, vector<ConstantSP>& arguments);
ConstantSP sql(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP sqlUpdate(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP sqlDelete(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP evaluate(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP objectComponent(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP objectType(Heap* heap,vector<ConstantSP>& arguments);

ConstantSP getChunkPath(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP sqlDS(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP textFilesDS(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP textChunkDS(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cacheDSNow(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP clearDSCacheNow(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP saveDSToDB(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cacheDS(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP clearDSCache(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP transDS(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP scheduleDS(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mrDS(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP imrDS(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP pipeline(Heap* heap,vector<ConstantSP>& arguments);

ConstantSP readTableFromFileSegment(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP readTextFromFileSegment(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP readTabletChunk(Heap* heap, vector<ConstantSP>& arguments);

ConstantSP partial(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP piecewise(Heap* heap,vector<ConstantSP>& arguments);

ConstantSP unifiedCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP delayedFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP partitionFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP dynamicFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP pdynamicFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ptableCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP loopFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ploopFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP eachFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ceachFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP peachFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP crossFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP pcrossFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP eachLeftFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP eachRightFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP eachPreFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP eachPostFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP rowReduceFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP reduceFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP accumulateFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP groupFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP rowGroupFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP contextFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP segmentFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP pivotFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP movingFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP rollingFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP allFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP anyFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP fillopFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP byRowFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP byColumnFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP talibFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP compareFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tmovingFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP aggrTopNFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP windowFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP twindowFuncCall(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP adverbFuncCall(Heap* heap,vector<ConstantSP>& arguments);

ConstantSP ej(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP sej(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP lj(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP rj(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP lsj(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP slsj(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP fj(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP aj(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP wj(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP pwj(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cj(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP pj(Heap* heap,vector<ConstantSP>& arguments);

ConstantSP pdfF(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP pdfChiSquare(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP pdfNormal(Heap* heap,vector<ConstantSP>& arguments);

ConstantSP cdfStudent(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cdfF(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cdfBeta(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cdfGamma(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cdfChiSquare(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cdfNormal(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cdfExp(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cdfUniform(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cdfWeibull(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cdfZipf(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cdfLogistic(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cdfBinomial(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cdfPoisson(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cdfKolmogorov(Heap* heap,vector<ConstantSP>& arguments);

ConstantSP invStudent(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP invF(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP invBeta(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP invGamma(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP invChiSquare(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP invNormal(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP invExp(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP invUniform(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP invWeibull(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP invZipf(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP invLogistic(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP invBinomial(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP invPoisson(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP invKolmogorov(Heap *heap, vector<ConstantSP> &arguments);

ConstantSP randStudent(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP randF(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP randBeta(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP randGamma(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP randChiSquare(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP randNormal(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP randMultivariateNormal(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP randExp(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP randUniform(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP randWeibull(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP randZipf(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP randLogistic(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP randBinomial(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP randPoisson(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP randKolmogorov(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP randDiscrete(Heap *heap, vector<ConstantSP> &arguments);

ConstantSP bondDuration(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP bondConvexity(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP bondDirtyPrice(Heap *heap, vector<ConstantSP> &arguments);

ConstantSP rowMin(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowMax(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowImin(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowImax(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowIminLast(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowImaxLast(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowAt(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowSum(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowSum2(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowCount(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowSize(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowStd(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowStdp(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowVar(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowVarp(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowSkew(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowKurtosis(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowAvg(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowAnd(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowOr(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowXor(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowProd(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowMed(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowRank(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowDenseRank(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowRatios(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowDeltas(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowPercentChange(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowZscore(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowDemean(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowCumsum(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowCumwsum(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowCumprod(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowCummax(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowCummin(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowMove(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowNext(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowPrev(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowCorr(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowCovar(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowBeta(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowWsum(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowWavg(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowTanimoto(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowJaccard(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowEuclidean(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowDot(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP rowAlign(Heap *heap, vector<ConstantSP> &arguments);

ConstantSP coalesce(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP panel(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP unionAll(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP conditionalFilter(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP trueRange(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP isortTop(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP partition(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP sample(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP freq(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP normal(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP digitize(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP bucket(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP bucketCount(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cutPoints(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP wcovariance(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ns(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP unpivot(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ungroup(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP rank(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP denseRank(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP isortInPlace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP joinInPlace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP removeHead(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP removeTail(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP iif(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP iterate(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP conditionalIterate(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP stateIterate(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP replace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ffill(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP bfill(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP lfill(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ffillInPlace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP bfillInPlace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP lfillInPlace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP winsorize(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP winsorizeInPlace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP clip(Heap *heap, vector<ConstantSP> &args);
ConstantSP clipInplace(Heap *heap, vector<ConstantSP> &args);
ConstantSP locate(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP slice(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP sliceByKey(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cell(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cells(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP array(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP bigarray(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP matrix(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP repmat(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP set(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP dictionary(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP syncDictionary(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP table(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP keyedTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP latestKeyedTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP indexedTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP latestIndexedTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP streamTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP keyedStreamTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP mvccTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP cachedTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP database(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP addValuePartitions(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP addRangePartitions(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tableInsert(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP getTablet(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP svd(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP qr(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP asfreq(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP resample(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP freqSeq(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP regroup(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP align(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP merge(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP eqFloat(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP oneHot(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP fixedLengthArrayVector(Heap* heap, vector<ConstantSP>& arguments);

ConstantSP exists(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP existsDatabase(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP existsTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP existsPartition(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP listTables(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP getTables(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP parseJsonTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP extractTextSchema(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP loadText(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ploadText(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP loadTextEx(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP loadTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP renameTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP loadColumn(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP loadStreamTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP loadMvccTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP createPartitionedTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP createTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP createIMOLTPTable(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP loadTableBySQL(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP bigSQL(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP loadRecord(Heap* heap,vector<ConstantSP>& arguments);

ConstantSP coevent(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP covarMatrix(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP corrMatrix(Heap* heap,vector<ConstantSP>& arguments);

ConstantSP objAddr(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP objects(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP objByName(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP funcByName(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP defined(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP functions(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP files(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP schema(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP objCall(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP unifiedObjCall(Heap *heap, vector<ConstantSP> &arguments);

ConstantSP refCount(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP xdb(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP pnodeRun(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP remoteRun(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP remoteUrgentRun(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP remoteRunWithCompression(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP remoteRunCompatible(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP rpc(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP socket(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP file(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP writeObject(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP readObject(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP write(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP writeLine(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP writeLines(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP read(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP readLinesInPlace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP seek(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP writeRecord(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP readRecordInPlace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP clearCache(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP getDiskIOStat(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP getRunningQueries(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP getCompletedQueries(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP getSessionMemoryStat(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP getMemoryStat(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP transaction(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP checksum(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP toCharArray(Heap* heap, vector<ConstantSP>& arguments);

ConstantSP transFreq(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP temporalSeq(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP convertTZ(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP temporalAdd(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP dailyAlignedBar(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP businessDay(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP monthBegin(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP monthEnd(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP businessMonthBegin(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP businessMonthEnd(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP quarterBegin(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP quarterEnd(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP businessQuarterBegin(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP businessQuarterEnd(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP yearBegin(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP yearEnd(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP businessYearBegin(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP businessYearEnd(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP semiMonthBegin(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP semiMonthEnd(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP weekOfMonth(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP lastWeekOfMonth(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP weekBegin(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP weekEnd(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP fy5253(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP fy5253Quarter(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP marketHoliday(Heap* heap, vector<ConstantSP>& arguments);

ConstantSP dropna(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP fillInPlace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP nullFillInPlace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP shuffleInPlace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP replaceInPlace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP sortInPlace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP append(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP upsert(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP tableUpsert(Heap *heap, vector<ConstantSP> &arguments);
ConstantSP pop(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP erase(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP update(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP sortBy(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ajInPlace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP ljInPlace(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP clear(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP rename(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP dictUpdate(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP reorderColumns(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP replaceColumn(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP dropColumns(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP setIndexedMatrix(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP setIndexedSeries(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP setColumnarTuple(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP appendTuple(Heap* heap,vector<ConstantSP>& arguments);

ConstantSP eqObj(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP license(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP updateLicense(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP genLicenseAuthorization(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP readLicenseAuthorization(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP shell(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP loadPlugin(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP loadPatch(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP chunkMeta(Heap* heap, vector<ConstantSP>& arguments);

ConstantSP submitJob(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP submitJobEx(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP submitJobEx2(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP getRecentJobs(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP getJobStatus(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP getJobReturn(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP getJobMessage(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP getConsoleJobs(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP evalTimer(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP getPartitionDataFromDS(Heap* heap, vector<ConstantSP>& arguments);

//reduce and running functions for map-reduce of aggregate functions
ConstantSP avgReduce(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP stdReduce(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP stdpReduce(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP varReduce(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP varpReduce(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP corrReduce(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP covarReduce(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP skewReduce(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP skewReduce2(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP kurtosisReduce(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP kurtosisReduce2(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP stdRunning(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP stdpRunning(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP varRunning(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP varpRunning(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP corrRunning(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP covarRunning(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP skewRunning(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP skewRunning2(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP kurtosisRunning(Heap* heap,vector<ConstantSP>& arguments);
ConstantSP kurtosisRunning2(Heap* heap,vector<ConstantSP>& arguments);

//system procedures
void sleep(Heap* heap,vector<ConstantSP>& arguments);
void print(Heap* heap,vector<ConstantSP>& arguments);
void writeLog(Heap* heap,vector<ConstantSP>& arguments);
void run(Heap* heap,vector<ConstantSP>& arguments);
void runScript(Heap* heap,vector<ConstantSP>& arguments);
void test(Heap* heap,vector<ConstantSP>& arguments);
void testCommitFailure(Heap* heap,vector<ConstantSP>& arguments);
void saveText(Heap* heap,vector<ConstantSP>& arguments);
void saveTable(Heap* heap,vector<ConstantSP>& arguments);
void savePartition(Heap* heap,vector<ConstantSP>& arguments);
void saveDualPartition(Heap* heap,vector<ConstantSP>& arguments);
void enableActivePartition(Heap* heap,vector<ConstantSP>& arguments);
void disableActivePartition(Heap* heap,vector<ConstantSP>& arguments);
void dropPartition(Heap* heap,vector<ConstantSP>& arguments);
void dropTable(Heap* heap,vector<ConstantSP>& arguments);
void saveDatabase(Heap* heap,vector<ConstantSP>& arguments);
void dropDatabase(Heap* heap,vector<ConstantSP>& arguments);
void addColumn(Heap* heap,vector<ConstantSP>& arguments);
void dropColumn(Heap* heap,vector<ConstantSP>& arguments);
void setColumnComment(Heap* heap,vector<ConstantSP>& arguments);
void setTableComment(Heap* heap,vector<ConstantSP>& arguments);
void setRetentionPolicy(Heap* heap,vector<ConstantSP>& arguments);
void setRemoveSpecialChar(Heap* heap,vector<ConstantSP>& arguments);
void setDatabaseForClusterReplication(Heap* heap,vector<ConstantSP>& arguments);
void setDatabaseClusterReplicationExecutionSet(Heap* heap,vector<ConstantSP>& arguments);
void setAtomicLevel(Heap* heap,vector<ConstantSP>& arguments);
void setRandomSeed(Heap* heap,vector<ConstantSP>& arguments);
void setMvccColumnDefaultValue(Heap* heap,vector<ConstantSP>& arguments);
void setMvccColumnNullability(Heap* heap,vector<ConstantSP>& arguments);
void close(Heap* heap,vector<ConstantSP>& arguments);
void mkdir(Heap* heap,vector<ConstantSP>& arguments);
void rmdir(Heap* heap,vector<ConstantSP>& arguments);
void rm(Heap* heap,vector<ConstantSP>& arguments);
void setSystem(Heap* heap,vector<ConstantSP>& arguments);
void setSQLStandard(Heap* heap,vector<ConstantSP>& arguments);
void addFunctionalViewInternal(Heap* heap,vector<ConstantSP>& arguments);
void updateFunctionViewOnDatanode(Heap* heap,vector<ConstantSP>& arguments);
void dropFunctionalViewInternal(Heap* heap,vector<ConstantSP>& arguments);
void undef(Heap* heap,vector<ConstantSP>& arguments);
void plot(Heap* heap,vector<ConstantSP>& arguments);
void plotHist(Heap* heap,vector<ConstantSP>& arguments);
void generateLicense(Heap* heap, vector<ConstantSP>& arguments);
void generatePluginSign(Heap* heap, vector<ConstantSP>& arguments);
void clearAllCache(Heap* heap, vector<ConstantSP>& arguments);
void clearCachedDatabase(Heap* heap, vector<ConstantSP>& arguments);
void clearDatabaseDomain(Heap* heap, vector<ConstantSP>& arguments);
void clearCachedSegment(Heap* heap, vector<ConstantSP>& arguments);
void checkMemoryUsageWarning(Heap* heap, vector<ConstantSP>& arguments);
void fflush(Heap* heap, vector<ConstantSP>& arguments);
void notifyWriteTask(Heap* heap, vector<ConstantSP>& arguments);
void reportConcurrentTasks(Heap* heap, vector<ConstantSP>& arguments);
void checkCtrAliveForConcurrentWrite(Heap* heap, vector<ConstantSP>& arguments);
void clearAllConcurrentWriteTasks(Heap* heap, vector<ConstantSP>& arguments);

void closeTransaction(Heap* heap, vector<ConstantSP>& arguments);
void commitTransaction(Heap* heap, vector<ConstantSP>& arguments);
void rollbackTransaction(Heap* heap, vector<ConstantSP>& arguments);
void cancelJob(Heap* heap, vector<ConstantSP>& arguments);
void cancelConsoleJob(Heap* heap, vector<ConstantSP>& arguments);
void cancelConsoleJobTask(Heap* heap, vector<ConstantSP>& arguments);
void addChunkMetaOnMaster(Heap* heap, vector<ConstantSP>& arguments);
void deleteChunkMetaOnMaster(Heap* heap, vector<ConstantSP>& arguments);
void saveColumn(Heap* heap, vector<ConstantSP>& arguments);
void updateTabletChunk(Heap* heap, vector<ConstantSP>& arguments);
void loadModule(Heap* heap, vector<ConstantSP>& arguments);
void saveModule(Heap* heap, vector<ConstantSP>& arguments);
ConstantSP uploadModule(Heap* heap, vector<ConstantSP>& arguments);
void appendMsg(Heap* heap,vector<ConstantSP>& arguments);
void generateMachineFingerprint(Heap *heap, vector<ConstantSP> &args);
void composeMachineFingerprint(Heap *heap, vector<ConstantSP> &args);

ConstantSP doubleSortPrefixJoin(TableSP left, TableSP right, ConstantSP leftMatchingColNames, ConstantSP rightMatchingColNames, ConstantSP leftFilter,ConstantSP rightFilter);
ConstantSP doubleSortEqualJoin(TableSP left, TableSP right, ConstantSP leftMatchingColNames, ConstantSP rightMatchingColNames, ConstantSP leftFilter,ConstantSP rightFilter, bool sortJoinedTable);
ConstantSP rightSortEqualJoin(TableSP left, TableSP right, ConstantSP leftMatchingColNames, ConstantSP rightMatchingColNames, ConstantSP leftFilter,ConstantSP rightFilter);
ConstantSP hashEqualJoin(TableSP left, TableSP right, ConstantSP leftMatchingColNames, ConstantSP rightMatchingColNames, ConstantSP leftFilter, ConstantSP rightFilter);
ConstantSP doubleSortLeftJoin(TableSP left, TableSP right,ConstantSP leftMatchingColNames, ConstantSP rightMatchingColNames, ConstantSP leftMatch, ConstantSP rightMatch, ConstantSP leftFilter,ConstantSP rightFilter);
ConstantSP doubleSortLeftSemiJoin(TableSP left, TableSP right,ConstantSP leftMatchingColNames, ConstantSP rightMatchingColNames, ConstantSP leftMatch, ConstantSP rightMatch, ConstantSP leftFilter,ConstantSP rightFilter, bool sortJoinedTable);
ConstantSP rightSortLeftSemiJoin(TableSP left, TableSP right, ConstantSP leftMatchingColNames, ConstantSP rightMatchingColNames, ConstantSP leftMatch, ConstantSP rightMatch, ConstantSP leftFilter,ConstantSP rightFilter);
ConstantSP doubleSortAsofJoin(TableSP left, TableSP right, ConstantSP leftMatchingColNames, ConstantSP rightMatchingColNames, ConstantSP leftFilter,ConstantSP rightFilter, bool sortJoinedTable);
ConstantSP hashMultipleColumnLeftSemiJoin(TableSP left, TableSP right, ConstantSP leftMatchingColNames, ConstantSP rightMatchingColNames, ConstantSP leftMatch, ConstantSP rightMatch, ConstantSP leftFilter,ConstantSP rightFilter);
ConstantSP hashLeftSemiJoin(TableSP left, TableSP right, const string& leftMatchingColName, const string& rightMatchingColName, ConstantSP leftMatch, ConstantSP rightMatch, ConstantSP leftFilter,ConstantSP rightFilter, bool sortJoinedTable);
ConstantSP hashLeftJoin(TableSP left, TableSP right, vector<int>& leftKeys, vector<int>& rightKeys, ConstantSP leftMatch, ConstantSP leftFilter);
ConstantSP doubleSortFullJoin(TableSP left, TableSP right, ConstantSP leftMatchingColNames, ConstantSP rightMatchingColNames, ConstantSP leftMatch, ConstantSP rightMatch, ConstantSP filterLeftSP,ConstantSP filterRightSP);


bool isAllTrue(const ConstantSP& obj);
INDEX checkVectorSize(const ConstantSP& a, const ConstantSP& b);
INDEX checkVectorSize(const ConstantSP& a, const ConstantSP& b, ConstantSP& a1, ConstantSP& b1);
void checkArgumentSize(const FunctionDefSP& func, int actualArgCount);
DATA_TYPE checkDataType(const ConstantSP& a);
void checkTupleType(const ConstantSP& tuple, DATA_TYPE type);
ConstantSP getColumn(const TableSP& table, const string& colName, const ConstantSP& filter);
ConstantSP getColumn(const TableSP& table, const string& colName);
string stripColumnQualifier(const string& colName);
void checkJoinColumnType(const TableSP& left, const TableSP& right, const ConstantSP& leftMatchingColNames, const ConstantSP& rightMatchingColNames);
void checkTupleReduce(const ConstantSP& init, const vector<ConstantSP>& in, vector<ConstantSP>& out, DATA_TYPE& type);
FunctionDefSP normalizePartialFunction(Heap* heap, const FunctionDefSP& partialFunc, vector<ConstantSP>& oldArgs, vector<ConstantSP>& newArgs);
FunctionDefSP normalizePartialFunction(Heap* heap, const FunctionDefSP& partialFunc, vector<ConstantSP>& oldArgs, vector<ConstantSP>& newArgs, vector<int>& oldArgPositions);
bool generateChunkPaths(Heap* heap, const DomainSP& domain, const ConstantSP& filters, vector<string>& paths, const string& tableName, string& errMsg);
/*method: 0-auto, 1-naive, 2-strassen*/
ConstantSP multiMatrix(const ConstantSP& a, const ConstantSP& b, int method=0);
VectorSP transposeVector(const ConstantSP& vec, INDEX rows, INDEX columns);
/* test if two objects are identical. */
bool testObject(const ConstantSP& a, const ConstantSP& b, double precision);
bool testFloatingVector(const ConstantSP& a, const ConstantSP& b, double precision);
bool testComplexVector(const ConstantSP& a, const ConstantSP& b, double precision);
ConstantSP hashBucket(const ConstantSP& keyObj, DATA_TYPE targetRawType, int buckets);

inline bool isTupleOfArray(const ConstantSP& a){
	return a->isTuple() && !((Vector*)a.get())->isTableColumn() && a->size() > 0 && a->get(0)->isArray();
}

inline bool isMultiColumn(const ConstantSP& a){
	return a->isMatrix() || a->isTable() || (a->isDictionary() && ((Dictionary*)a.get())->isOrdered()) || isTupleOfArray(a);
}

inline bool isMultiColumn2(const ConstantSP& a){
	return a->isMatrix() || a->isTable() || (a->isDictionary() && ((Dictionary*)a.get())->isOrdered());
}

inline bool isMultiColumn(const ConstantSP& a, const ConstantSP& b){
	return a->isMatrix() || a->isTable() || b->isMatrix() || b->isTable() || isTupleOfArray(a) || isTupleOfArray(b);
}

inline bool isMultiColumn2(const ConstantSP& a, const ConstantSP& b){
	return a->isMatrix() || a->isTable() || b->isMatrix() || b->isTable();
}

inline bool isLabeledMultiColumn(const ConstantSP& a){
	return a->isIndexed() || a->isSeries();
}

inline bool isLabeledMultiColumn(const ConstantSP& a, const ConstantSP& b){
	return (a->isIndexed() || a->isSeries()) && (b->isIndexed() || b->isSeries());
}

inline bool isVectorDerived(const ConstantSP& a) {
	DATA_FORM df = a->getForm();
	DATA_TYPE type = a->getType();
	return df == DF_TABLE || df == DF_DICTIONARY || (df == DF_VECTOR && (type >= ARRAY_TYPE_BASE || type == DT_ANY));
}

inline bool isVectorDerived(const ConstantSP& a, const ConstantSP& b) {
	DATA_FORM df = a->getForm();
	DATA_TYPE type = a->getType();
	if(df == DF_TABLE || df == DF_DICTIONARY || (df == DF_VECTOR && (type >= ARRAY_TYPE_BASE || type == DT_ANY)))
		return true;
	df = b->getForm();
	type = b->getType();
	return df == DF_TABLE || df == DF_DICTIONARY || (df == DF_VECTOR && (type >= ARRAY_TYPE_BASE || type == DT_ANY));
}
typedef ConstantSP(*CompareFunc)(const ConstantSP&,const ConstantSP&, bool nullAsMinValue);
/**
 * The semanticCategory for computeUnary and computeBinary
 * 0: process all types of columns in a table
 * 1: process numeric columns in a table
 * 2: process temporal columns in a table
 * 3: process string columns in a table
 * 4: process numeric and temporal columns in a table
 *
 * By default, only process numeric columns in a table
 */
ConstantSP computeUnary(const ConstantSP& a, const ConstantSP& b, OptrFunc optr, int semanticCategory = 1, int objIndex = 0);
ConstantSP computeUnary(Heap* heap, vector<ConstantSP>& args, SysFunc optr, int semanticCategory = 1, int objIndex= -1);
ConstantSP computeBinary(const ConstantSP& a, const ConstantSP& b, OptrFunc optr, int semanticCategory = 1);
ConstantSP eachColumn(const ConstantSP& a, const ConstantSP& b, OptrFunc optr, int semanticCategory = 1, bool isAggregate = false, bool isPair = false, bool isVector = true);
ConstantSP eachColumn(const ConstantSP& a, const ConstantSP& b, OptrFunc optr, FastFunc fastFunc, int semanticCategory = 1, bool isAggregate = false, bool isPair = false, bool isVector = true);
ConstantSP eachColumn(Heap* heap, vector<ConstantSP>& args, SysFunc optr, int secondMatrixIndex, int semanticCategory = 1, bool isAggregate = false, bool isPair = false);
template<class T>
ConstantSP eachMatrix(Heap* heap, vector<ConstantSP>& args, T optr, FastFunc fastFunc, int secondMatrixIndex, bool aggregate);
ConstantSP eachTable(Heap* heap, vector<ConstantSP>& args, const FunctionDefSP& optr, int secondTableIndex, int semanticCategory=1);
ConstantSP eachColumn(Heap* heap, vector<ConstantSP>& args, SysFunc optr, int secondMatrixIndex, int thirdMatrixIndex, int semanticCategory);
ConstantSP eachColumnInPlace(Heap* heap, vector<ConstantSP>& args, SysFunc optr, int semanticCategory = 1, int objIndex = -1);

template<class T>
ConstantSP rowArrayVector(Heap* heap, vector<ConstantSP>& args, T optr, FastFunc fastFunc, bool aggregate);
template<class T>
ConstantSP rowArrayVector(Heap* heap, vector<ConstantSP>& args, T optr, FastFunc fastFunc, int secondArrayIndex, bool aggregate);
template<class T>
ConstantSP rowTuple(Heap* heap, vector<ConstantSP>& args, T optr, FastFunc fastFunc, int secondTupleIndex, bool aggregate);
template<class T>
ConstantSP rowWiseProcess(Heap* heap, vector<ConstantSP>& args, T optr, FastFunc fastFunc, bool aggregate, const string& name, const string& syntax);
/**
 * row-level calculation on matrix.
 * solutions: (1) transpose the matrix, (2) column-level calculation, (3) transpose the resulting matrix
 */
ConstantSP rowMatrix(Heap* heap, vector<ConstantSP>& args, SysFunc optr);
ConstantSP matrixJoin(const ConstantSP& a, const ConstantSP& b, bool indexedMatrix, bool useLeftLabel = true);
ConstantSP rollMultiColumns(Heap* heap, const ColumnContextSP& colContext, WindowJoinFunction& windowFunction, const vector<ConstantSP>& args,
		const string& funcName, bool binary = false, INDEX minPeriodArgIndex = -1);
ConstantSP rollVector(Heap* heap, WindowJoinFunction& windowFunction, const vector<ConstantSP>& args, const string& funcName, const string& syntax, bool binary = false, std::pair<long long, long long> excludedPeriod = {-1, -1});
ConstantSP forwardRollVector(Heap* heap, WindowJoinFunction& windowFunction, const vector<ConstantSP>& args, const string& funcName, const string& syntax, bool binary = false);
ConstantSP computeArrayVector(const ConstantSP& a, const ConstantSP& b, OptrFunc optr);
VectorSP copyIndexVectorOfArrayVector(const VectorSP& index);
bool checkArrayVectorSize(const ConstantSP& a, const ConstantSP& b);

}

#endif /* OPERATORIMP_H_ */
