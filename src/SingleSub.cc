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

#include <cassert>
#include <Sequence/Comparisons.hpp>
#include <Sequence/RedundancyCom95.hpp>
#include <Sequence/SingleSub.hpp>
#include <Sequence/Translate.hpp>
#include <sstream>
using namespace std;

namespace Sequence
{
  struct SingleSub::SingleSubImpl
  {
    double q0i, q2Si, q2Vi, q4i, q0j, q2Sj, q2Vj, q4j, p0i, p2Si, p2Vi,
      p4i, p0j, p2Sj, p2Vj, p4j;
    void Calculate (const RedundancyCom95 & sitesObj, const std::string & cod1,
		    const std::string & cod2);
    SingleSubImpl(): q0i(0.), q2Si(0.), q2Vi(0.), q4i(0.), q0j(0.), q2Sj(0.), q2Vj(0.), q4j(0.), p0i(0.), p2Si(0.), p2Vi(0.),
		     p4i(0.), p0j(0.), p2Sj(0.), p2Vj(0.), p4j(0.)
    {
    }
  };
  
  SingleSub::SingleSub(void) : impl(std::unique_ptr<SingleSub::SingleSubImpl>(new SingleSub::SingleSubImpl()))
  {
  }

  SingleSub::~SingleSub()
  {
  }

  void SingleSub::operator() (const RedundancyCom95 & sitesObj,
			      const std::string & cod1,
			      const std::string & cod2)
  /*!
    \param sitesObj an object of type Sequence::RedundancyCom95
    \param cod1 a std::string of length 3 representing a codon
    \param cod2 a std::string of length 3 representing a codon
    \note cod1 and cod2 lengths are verified by assert()
  */
  {
    assert(cod1.length() == 3 && cod2.length() == 3);
    impl->Calculate (sitesObj, cod1, cod2);
  }

  double SingleSub::P0(void) const
  /*!
    \return number of transitions at non-degenerate sites in the codon
  */
  {
    return (impl->p0i+impl->p0j)/2.0;
  }


  double SingleSub::P2S(void) const
  /*!
    \return number of transitions at transitional-degenerate sites in the codon
  */
  {
    return (impl->p2Si+impl->p2Sj)/2.0;
  }


  double SingleSub::P2V(void) const
  /*!
    \return number of transitions at transversional-degenerate sites in the codon
  */
  {
    return (impl->p2Vi+impl->p2Vj)/2.0;
  }


  double SingleSub::P4(void) const
  /*!
    \return number of transitions at fourfold-degenerate sites in the codon
  */
  {
    return (impl->p4i+impl->p4j)/2.0;
  }

  double SingleSub::Q0(void) const
  /*!
    \return number of transversions at non-degenerate sites in the codon
  */
  {
    return (impl->q0i+impl->q0j)/2.0;
  }


  double SingleSub::Q2S(void) const
  /*!
    \return number of transversions at transitional-degenerate sites in the codon
  */
  {
    return (impl->q2Si+impl->q2Sj)/2.0;
  }


  double SingleSub::Q2V(void) const
  /*!
    \return number of transversions at transversional-degenerate sites in the codon
  */
  {
    return (impl->q2Vi+impl->q2Vj)/2.0;
  }


  double SingleSub::Q4(void) const
  /*!
    \return number of transversions at fourfold-degenerate sites in the codon
  */
  {
    return (impl->q4i+impl->q4j)/2.0;
  }

  void
  SingleSub::SingleSubImpl::Calculate (const RedundancyCom95 & sitesObj, const std::string & codon1,
				       const std::string & codon2)
  /*!
    count up mutations between the codons
  */
  {
    q0i = q2Si = q2Vi = q4i = q0j = q2Sj = q2Vj = q4j = p0i = p2Si =
      p2Vi = p4i = p0j = p2Sj = p2Vj = p4j = 0.0;

    unsigned pos = 0, k = 0;//, type = 0;
    double d;
    for (k = 0; k <= 2; ++k)
      {
	if (codon1[k] != codon2[k])
	  {
	    pos = k;
	  }
      }
    Mutations type = TsTv (codon1[pos], codon2[pos]);
    if( type == Mutations::Unknown )
      {
	ostringstream o;
	o << "SingleSub.cc: mutation between " << codon1 << " and " << codon2
	  << " at position " << pos
	  << " is neither a transition nor a transversion";
	throw SeqException(o.str().c_str());
      }
    assert(type==Mutations::Ts || type==Mutations::Tv);

    switch (pos)
      {
      case 0: //mutation at first position
	switch (type)
	  {
	  case Mutations::Ts: //transition
	    d = sitesObj.FirstNon(codon1);
	    if( d < 1. )
	      {
		d = sitesObj.First2S(codon1);
		if(d > 0.)
		  {
		    p2Si += 1.;
		  }
		else
		  {
		    p2Vi += 1.;
		  }
	      }
	    else
	      {
		p0i += 1.;
	      }
	    d = sitesObj.FirstNon(codon2);
	    if( d < 1. )
	      {
		d = sitesObj.First2S(codon2);
		if(d > 0.)
		  {
		    p2Sj += 1.;
		  }
		else
		  {
		    p2Vj += 1.;
		  }
	      }
	    else
	      {
		p0j += 1.;
	      }
	    break;
	  case Mutations::Tv: //transversion
	    d = sitesObj.FirstNon(codon1);
	    if( d < 1. )
	      {
		d = sitesObj.First2V(codon1);
		if(d > 0.)
		  {
		    q2Vi += 1.;
		  }
		else
		  {
		    q2Si += 1.;
		  }
	      }
	    else
	      {
		q0i += 1.;
	      }
	    d = sitesObj.FirstNon(codon2);
	    if( d < 1. )
	      {
		d = sitesObj.First2V(codon2);
		if(d > 0.)
		  {
		    q2Vj += 1.;
		  }
		else
		  {
		    q2Sj += 1.;
		  }
	      }
	    else
	      {
		q0j += 1.;
	      }
	    break;
	  case Mutations::Unknown:
	    throw Sequence::SeqException( "SingleSub: mutation type unknown" );
	    break;
	  }
	break;
      case 1: //mutation at second position
	switch (type)
	  {
	  case Mutations::Ts: //transition
	    p0i += 1.0;
	    p0j += 1.0;
	    break;
	  case Mutations::Tv:
	    q0i += 1.0;
	    q0j += 1.0;
	    break;
	  case Mutations::Unknown:
	    throw Sequence::SeqException( "SingleSub: mutation type unknown" );
	    break;
	  }
	break;
      case 2: //mutation at third position
	switch (type)
	  {
	  case Mutations::Ts:
	    d = sitesObj.ThirdNon (codon1);
	    if ( d < 1.)
	      {
		d = sitesObj.ThirdFour (codon1);
		if ( d < 1. )
		  {
		    d = sitesObj.Third2S (codon1);
		    if ( d > 0. )
		      {
			p2Si += 1.;
		      }
		    else
		      {
		 	p2Vi += 1.;
		      }
		  } 
		else
		  {
		    p4i += 1.;
		  }
	      }
	    else
	      {
		p0i += 1.;
	      }

	    d = sitesObj.ThirdNon (codon2);
	    if ( d < 1.)
	      {
		d = sitesObj.ThirdFour (codon2);
		if ( d < 1. )
		  {
		    d = sitesObj.Third2S (codon2);
		    if ( d > 0. )
		      {
			p2Sj += 1.;
		      }
		    else
		      {
			p2Vj += 1.;
		      }
		  } 
		else
		  {
		    p4j += 1.;
		  }
	      }
	    else
	      {
		p0j += 1.;
	      }
	  
	    break;
	    //case 2: 
	  case Mutations::Tv: //transversion
	    d = sitesObj.ThirdNon (codon1);
	    if ( d < 1.)
	      {
		d = sitesObj.ThirdFour (codon1);
		if ( d < 1. )
		  {
		    d = sitesObj.Third2V (codon1);
		    if ( d > 0. )
		      {
			q2Vi += 1.;
		      }
		    else
		      {
			q2Si += 1.;
		      }
		  } 
		else
		  {
		    q4i += 1.;
		  }
	      }
	    else
	      {
		q0i += 1.;
	      }

	    d = sitesObj.ThirdNon (codon2);
	    if ( d < 1.)
	      {
		d = sitesObj.ThirdFour (codon2);
		if ( d < 1. )
		  {
		    d = sitesObj.Third2V (codon2);
		    if ( d > 0. )
		      {
			q2Vj += 1.;
		      }
		    else
		      {
			q2Sj += 1.;
		      }
		  } 
		else
		  {
		    q4j += 1.;
		  }
	      }
	    else
	      {
		q0j += 1.;
	      }
	  
	    break;
	  case Mutations::Unknown:
	    throw Sequence::SeqException( "SingleSub: mutation type unknown" );
	    break;
	  }
	break;
      }
  }
}
