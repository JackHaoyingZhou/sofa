/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_COMPONENT_ENGINE_GENERATERIGIDMASS_H
#define SOFA_COMPONENT_ENGINE_GENERATERIGIDMASS_H
#include "config.h"

#include <sofa/core/DataEngine.h>
#include <sofa/core/objectmodel/DataFileName.h>
#include <sofa/helper/io/Mesh.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/Vec.h>

namespace sofa
{

namespace component
{

namespace engine
{

template <class DataTypes, class MassType>
class GenerateRigidMass : public core::DataEngine
{
public:
    SOFA_CLASS(GenerateRigidMass,core::DataEngine);

    GenerateRigidMass();
    ~GenerateRigidMass() override;

    /// Initialization method called at graph modification, during bottom-up traversal.
    void init() override;
    /// Update method called when variables used in precomputation are modified.
    void reinit() override;
    /// Update the output values
    void doUpdate() override;

protected:

    typedef defaulttype::Vector3 Vector3;
    typedef helper::fixed_array <unsigned int,3> MTriangle;
    typedef helper::fixed_array <unsigned int,4> MQuad;
    typedef helper::vector <unsigned int> MPolygon;

    typedef typename DataTypes::Real Real;
    typedef typename DataTypes::Vec3 Vec3;
    typedef defaulttype::Mat<3,3,Real> Mat3x3;

    /**
      * Data Fields
      */
    /// input
    Data< Real > m_density; ///< kg * m^-3
    Data< helper::vector< Vector3 > > m_positions; ///< input: positions of the vertices
    Data< helper::vector< MTriangle > > m_triangles; ///< input: triangles of the mesh
    Data< helper::vector< MQuad > > m_quads; ///< input: quads of the mesh
    Data< helper::vector< MPolygon > > m_polygons; ///< must be convex

    /// output
    Data< MassType > rigidMass;
    Data< Real > mass; ///< output: mass of the mesh
    Data< Real > volume; ///< output: volume of the mesh
    Data < Mat3x3 > inertiaMatrix; ///< output: the inertia matrix of the mesh
    Data< Vec3 > massCenter; ///< output: the gravity center of the mesh
    Data< Vector3 > centerToOrigin; ///< output: vector going from the mass center to the space origin

    /**
      * Protected methods
      */
    /// integrates the whole mesh
    void integrateMesh();

    void integrateTriangle(Vector3 kV0,Vector3 kV1,Vector3 kV2);

    /// generates the RigidMass from the mesh integral
    void generateRigid();

    helper::fixed_array<SReal,10> afIntegral;

public:
    /// Implementing the GetCustomTemplateName is mandatory to have a custom template name paremters
    /// instead of the default one generated automatically by the SOFA_CLASS() macro.
    static std::string GetCustomTemplateName();

};

#if  !defined(SOFA_COMPONENT_ENGINE_GENERATERIGIDMASS_CPP)
extern template class SOFA_GENERAL_ENGINE_API GenerateRigidMass<defaulttype::Rigid3Types, defaulttype::Rigid3Mass>;

#endif

} // namespace engine

} // namespace component

} // namespace sofa

#endif
