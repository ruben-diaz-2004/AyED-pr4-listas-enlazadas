// AUTOR: Rubén Díaz Marrero
// FECHA: 10/04/2023
// EMAIL: alu0101552613@ull.edu.es
// VERSION: 2.0
// ASIGNATURA: Algoritmos y Estructuras de Datos
// PRÁCTICA Nº: 4
// ESTILO: Google C++ Style Guide
// COMENTARIOS:
// 

#ifndef SLLPOLYNOMIAL_H_
#define SLLPOLYNOMIAL_H_

#include <iostream>
#include <math.h>  // fabs, pow

#include "pair_t.h"
#include "sll_t.h"
#include "vector_t.h"

#define EPS 1.0e-6

typedef pair_t<double> pair_double_t;  // Campo data_ de SllPolynomial
typedef sll_node_t<pair_double_t> SllPolyNode;  // Nodos de SllPolynomial

// Clase para polinomios basados en listas simples de pares
class SllPolynomial : public sll_t<pair_double_t> {
 public:
  // constructores
  SllPolynomial(void) : sll_t() {};
  SllPolynomial(const vector_t<double>&, const double = EPS);

  // destructor
  ~SllPolynomial() {};

  // E/S
  void Write(std::ostream& = std::cout) const;
  
  // operaciones
  double Eval(const double) const;
  bool IsEqual(const SllPolynomial&, const double = EPS) const;
  void Sum(const SllPolynomial&, SllPolynomial&, const double = EPS);
};


bool IsNotZero(const double val, const double eps = EPS) {
  return fabs(val) > eps;
}

// FASE II
// constructor
SllPolynomial::SllPolynomial(const vector_t<double>& v, const double eps) {
  for (int i{v.get_size() -1}; i >= 0; --i) {
    pair_double_t par;
    par.set(v[i], i);
    if (IsNotZero(par.get_val(), eps)) {
      SllPolyNode poly_node(par);
      SllPolyNode* nodo;
      nodo = new SllPolyNode(poly_node);
      push_front(nodo);
    }
  }
  
}


// E/S
void SllPolynomial::Write(std::ostream& os) const {
  os << "[ ";
  bool first{true};
  SllPolyNode* aux{get_head()};
  while (aux != NULL) {
    int inx{aux->get_data().get_inx()};
    double val{aux->get_data().get_val()};
    if (val > 0)
      os << (!first ? " + " : "") << val;
    else
      os << (!first ? " - " : "-") << fabs(val);
    os << (inx > 1 ? " x^" : (inx == 1) ? " x" : "");
    if (inx > 1)
      os << inx;
    first = false;
    aux = aux->get_next();
  }
  os << " ]" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const SllPolynomial& p) {
  p.Write(os);
  return os;
}


// Operaciones con polinomios

// FASE III
// Evaluación de un polinomio representado por lista simple
double SllPolynomial::Eval(const double x) const {
  double result{0.0};
  SllPolyNode* aux = get_head();
  while (aux != NULL) {
    result += aux->get_data().get_val() * pow(x,aux->get_data().get_inx());
    
    aux = aux->get_next();
  }
  
  return result;
}

// Comparación si son iguales dos polinomios representados por listas simples
bool SllPolynomial::IsEqual(const SllPolynomial& sllpol, const double eps) const {
  bool differents = false;
  SllPolyNode* aux = get_head();
  SllPolyNode* aux_2 = sllpol.get_head();
  int contador{0};
  while (aux != NULL && aux_2 != NULL) {
    if (aux->get_next() == NULL && aux_2->get_next() != NULL) return false;
    else if (aux_2->get_next() == NULL && aux->get_next() != NULL) return false;

    if (fabs(Eval(contador) - sllpol.Eval(contador)) > eps) differents = true;
    aux = aux->get_next();
    aux_2 = aux_2->get_next();
    ++contador;
  }
  return !differents;
}

// FASE IV
// Generar nuevo polinomio suma del polinomio invocante mas otro polinomio
void SllPolynomial::Sum(const SllPolynomial& sllpol, SllPolynomial& sllpolsum, const double eps) {
  SllPolyNode* aux = get_head();
  SllPolyNode* aux_2 = sllpol.get_head();
  SllPolyNode* aux_result = sllpolsum.get_head();
  pair_double_t par;
  while (aux != NULL || aux_2 != NULL) {
    if (aux != NULL && aux_2 != NULL) {
      if (aux->get_data().get_inx() == aux_2->get_data().get_inx()) {
        par.set(aux->get_data().get_val() + aux_2->get_data().get_val(), aux->get_data().get_inx());
        aux = aux->get_next();
        aux_2 = aux_2->get_next();
        // SllPolyNode polynodo(par);
        // SllPolyNode* nuevo_nodo = new SllPolyNode(polynodo);
        // sllpolsum.insert_after(aux_prev, nuevo_nodo);
      } else if (aux->get_data().get_inx() > aux_2->get_data().get_inx()) {
          par.set(aux_2->get_data().get_val(), aux_2->get_data().get_inx());
          aux_2 = aux_2->get_next();
        } else {
          par.set(aux->get_data().get_val(), aux->get_data().get_inx());
          aux = aux->get_next();
        }
    } else if (aux != NULL) {
      par.set(aux->get_data().get_val(), aux->get_data().get_inx());
      aux = aux->get_next();
    } else {
      par.set(aux_2->get_data().get_val(), aux_2->get_data().get_inx());
      aux_2 = aux_2->get_next();
    }
    if (sllpolsum.get_head() == NULL) {
      sllpolsum.push_front(new SllPolyNode(par));
      aux_result = sllpolsum.get_head();
    } else {
      sllpolsum.insert_after(aux_result, new SllPolyNode(par));
      aux_result = aux_result->get_next();
    }
  }
}


#endif  // SLLPOLYNOMIAL_H_
