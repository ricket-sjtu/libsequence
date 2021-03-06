/*

Copyright (C) 2003-2009 Kevin Thornton, krthornt[]@[]uci.edu

Remove the brackets to email me.

This file is part of libsequence.

libsequence is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libsequence is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
long with libsequence.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef SITES_H
#define SITES_H
/*! \file Sites.hpp
  @brief class Sequence::Sites calculates the length of a pairwise alignment between coding
  sequences in terms of site degeneracy.  Used by Sequence::Comeron95
*/

/*!
  \class Sequence::Sites Sequence/Sites.hpp
  \ingroup kaks
  This class calculates the length of each sequence in a pairwise
  comparison in terms of site degeneracy.  The 4 values computed
  are:\n
  1.) L0 -- the number of non-degenerate sites in the comparison.\n
  2.) L2S -- the number of two-fold degenerate sites in the comparison
  for which transitions are synonymous.\n
  3.) L2V -- the number of two-fold degenerate sites in the comparison
  for which transversion are synonymous.\n
  4.) L4 -- the number of fourfold degenerate sites in the comparison.\n
  \n
  In order to count these numbers, one has to know how degenerate
  each codon is, which is why objects of this type must be constructed
  with objects of type Sequence::RedundancyCom95.
 
  @author Kevin Thornton
  @short Calculate length statistics for divergence calculations
*/

#include <string>
#include <memory>
namespace Sequence
  {
    class Seq;
    class RedundancyCom95;
    class Sites
    {
    private:
      struct SitesImpl;
      std::unique_ptr<SitesImpl> impl;
    public:
      explicit Sites ();
      explicit Sites(const Sequence::Seq & seq1,
		     const Sequence::Seq & seq2,
		     const RedundancyCom95 & sitesObj,
		     int maxdiffs = 3);
      Sites(Sites &&) = default;
      void operator()(const Sequence::Seq & seq1,
		      const Sequence::Seq & seq2,
		      const RedundancyCom95 & sitesObj,
		      int maxdiffs = 3);
      ~Sites (void);
      /*!
        \return alignment length in terms of non-degenerate sites
      */
      double L0(void) const;
      /*!
        \return alignment length in terms of transitional-degenerate sites
      */
      double L2S(void) const;
      /*!
        \return alignment length in terms of transversional-degenerate sites
      */
      double L2V(void) const;
      /*!
        \return alignment length in terms of fourfold-degenerate sites
      */
      double L4(void) const;
    };
}
#endif
