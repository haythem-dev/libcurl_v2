# APM-Supplement – Basar 1.11. db::aspect changes

## Introduction

This is a Basar APM-Supplement for the changes of Basars db::aspect layer starting of version 1.11. Herein you will find everything you need to know about using these features.

### Goals of the feature were:

- Enable the use of Prepared Statements in Basars db::aspect layer

### Additional changes:

- During development several performance increasing changes regarding data copying from record set to property table were implemented

## Usage

The needed header files are part of the header `“libbasardbaspect.h”`.

The Prepared Statement functionality adds “prepare-able” counterparts to the existing SQL String Builder classes and macros:

- `BEGIN_PREPAREABLE_QUERY_BUILDER_DECLARATION`
- `BEGIN_PREPAREABLE_SINGLE_QUERY_BUILDER_DECLARATION`
- `BEGIN_PREPAREABLE_AGGREGATE_QUERY_BUILDER_DECLARATION`
- `BEGIN_PREPAREABLE_WRITE_BUILDER_DECLARATION`

These String Builders function as their already known counterparts by adding member functions for `isExecuteable` and `buildSQLString` and creating and adding them to an AccessMethod.\
In `buildSQLString` quoting or transforming input values must not be done (akin to db::sqls prepared statement).\
Check the Basar APM for additional information on the use of the existing String Builder classes.

## Cost

While faster in execution (see Performance) db::aspects Prepared Statements come at a cost. As the SQL String Builder classes allow queries based on the presence or absence of data (by the use of `isContainedAndSet`), the resulting SQL string is stored to act as a key to the created prepared statement, which is also cached. Thus, a small overhead both in runtime (map lookup) and memory is required. The lifetime of these cached prepared statements is from the first execute of an AccessMethod with a prepared statement up to the destruction of the Accessor.

## Performance

For the following measurements, keep in mind the focus is not the exact value of time taken but the ratio between the different query methods. Differences between debug and release are of no concern and result from different system load states at the time of measuring. Also it is expected, that debug and release measurements for db::sql are about the same.

### Performing 1000 Inserts into database (Windows):

|                  | db::sql (prepared stmt) | db::aspect (prepared stmt) | db::sql (normal) | db::aspect (normal) |
|------------------|:-----------------------:|:--------------------------:|:----------------:|:-------------------:|
| Debug (in sec)   | 17,2s                   | 20,4s                      | 34,8s            | 37,5s               |
| Release (in sec) | 16,9s                   | 18,4s                      | 34,5s            | 35,8s               |
| Speedup debug    | Baseline                | +18%                       | +102%            | +118%               |
| Speedup release  | Baseline                | +8%                        | +104%            | +111%               |

For the select measurements, the caching policy was set to NO\_CACHING in order to get comparable values. Furthermore, no resulting values where accessed. Db::sqls Resultset was accessed as db::aspect does this irregardless of caching policy.

### Performing 1000 Selects from database (Windows):

|                      | db::sql (prepared stmt) | db::aspect (prepared stmt) | db::sql (normal) | db::aspect (normal)  |
|----------------------|:-----------------------:|:--------------------------:|:----------------:|:--------------------:|
|  Debug (in sec)      | 34,7s                   | 37,6s                      |  103,9s          | 104,3s               |
|  Release (in sec)    | 34,6s                   | 35,7s                      |  99,7s           | 100,4s               |
|  Speedup debug       | Baseline                | +8%                        |  +199%           | +200%                |
|  Speedup release     | Baseline                | +3%                        |  +188%           | +190%                |

<!-- markdownlint-disable MD033 -->
<div style="page-break-before: always;">

### DB-Transfer Performance

Transferring data from record set to property table is now about 25%+ faster than before. Guidelines for proper usage of caching policies as outlined in the Basar APM are still in place.

|                     |  NO\_CACHING  | SINGLE\_ROW  | ON\_DEMAND  | FULL    |
|---------------------|:-------------:|:------------:|:-----------:|:-------:|
|  Basar 01.10.01.00  |  1,03s        |  10,2s       |  13,5s      |  16,3s  |
|  Basar 01.11.pre    |  1,03s        |  7,2s        |  8,6s       |  12,2s  |
<!-- markdownlint-disable MD036  -->
*Select and Iterate over 100k datasets on Unix, measurements in seconds*
</div>