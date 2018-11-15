#ifndef BART_SRC_EQUATION_DIFFUSION_H_
#define BART_SRC_EQUATION_DIFFUSION_H_

#include "equation_base.h"

//! This class provides weak formulation for even-parity equation.
/*!
 The governing equation for this class is the diffusion equation. Currently CFEM
 formulation is implemented. DFEM-diffusion will be developed as well in the future.

 The purposes of this class are: providing diffusion formulation as a space-angle
 solver; serving as the base class for any diffusion-like formulations such as
 NDA and DSA

 \author Weixiong Zheng, Joshua Rehak
 \date 2018/11
 */
template <int dim>
class Diffusion : public EquationBase<dim> {
 public:
  /*!
   Class constructor.

   \param equation_name An abbreviated name of the equation.
   \param prm ParameterHandler object
   */
  Diffusion (const std::string &equation_name,
      const dealii::ParameterHandler &prm,
      std::shared_ptr<FundamentalData<dim>> &dat_ptr);

  /*!
   Class destructor.
   */
  virtual ~Diffusion ();

  /*!
   This function overrides the one in EquationBase. The functionality includes
   diffusion part assembly and closure assembly. Diffusion part assembly includes.

   \return Void.
   */
  virtual virtual void PreassembleCellMatrices ();

  /*!
   This function provides cell-wise integrator for bilinear form. Specifically,
   this overrides EquationBase<dim>'s integrator for even parity equation. Note
   that interface and boundary face terms are not handled in this integrator.

   \param cell Active iterator used to iterating cells.
   \param cell_matrix Local matrix in bilinear form to be modified.
   \param g Group index.
   \param i_dir Direction index.
   \return Void.
   */
  virtual void IntegrateCellBilinearForm (
      typename dealii::DoFHandler<dim>::active_cell_iterator &cell,
      dealii::FullMatrix<double> &cell_matrix,
      const int &g,
      const int &dir);

  /*!
   This function provides cellwise integrator for linear form assembly specifically
   for the contribution of scattering.

   \param cell Active cell iterator containing cell info.
   \param cell_rhs Local vector (linear form) to be modified.
   \param sflxes_proc Scalar fluxes for all groups living on current processor.
   \param g Group index.
   \param i_dir Direction index.
   \return Void.
   */
  virtual void IntegrateScatteringLinearForm (
      typename dealii::DoFHandler<dim>::active_cell_iterator &cell,
      dealii::Vector<double> &cell_rhs,
      const int &g,
      const int &dir);

  /*!
   This function provides cellwise integrator for linear form assembly specifically
   for the contribution of fixed source in fixed-source problems or fission in
   eigenvalue problems.

   \param cell Active cell iterator containing cell info.
   \param cell_rhs Local vector (linear form) to be modified.
   \param sflxes_prev Scalar fluxes from previous generation due to fission for
   all groups living on current processor.
   \param g Group index.
   \param i_dir Direction index.
   \return Void.

   \note sflxes_prev will do nothing inside the integrator fixed source problems.
   */
  virtual void IntegrateCellFixedLinearForm (
      typename dealii::DoFHandler<dim>::active_cell_iterator &cell,
      dealii::Vector<double> &cell_rhs,
      const int &g,
      const int &dir);

  /*!
   This function provides cellwise integrator for bilinear form assembly on
   boundary.

   \param cell Active cell iterator containing cell info.
   \param fn Face index in current cell for current boundary face.
   \param cell_matrix Local cell matrix to be modified in current cell.
   \param g Group index.
   \param dir Direction index set to zero for diffusion.
   \return Void.
   */
  void IntegrateBoundaryBilinearForm (
      typename dealii::DoFHandler<dim>::active_cell_iterator &cell,
      const int &fn,/*face number*/
      dealii::FullMatrix<double> &cell_matrix,
      const int &g,
      const int &dir);

protected:
  //! Polynomial order related part of penalty coefficient.
  double c_penalty_;

  //! Preassembled streaming matrices at quadrature points for diffusion
  std::unordered_map<int, dealii::FullMatrix<double>> pre_streaming_;
};

#endif // BART_SRC_EQUATION_DIFFUSION_H_
