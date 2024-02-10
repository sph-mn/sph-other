(declare
  sp-cheap-filter-state-t
  (type
    (struct
      (in-temp sp-sample-t*)
      (out-temp sp-sample-t*)
      (svf-state (array sp-sample-t ((* 2 sp-filter-passes-limit))))))
  (sp-cheap-filter type in in-size cutoff passes q-factor state out)
  (void sp-state-variable-filter-t sp-sample-t*
    sp-time-t sp-sample-t sp-time-t sp-sample-t sp-cheap-filter-state-t* sp-sample-t*)
  (sp-cheap-filter-state-free a) (void sp-cheap-filter-state-t*)
  (sp-cheap-filter-state-new max-size passes out)
  (status-t sp-time-t sp-filter-passes-t sp-cheap-filter-state-t**)
  (sp-state-variable-filter-lp out in in-count cutoff q-factor state)
  (void sp-sample-t* sp-sample-t* sp-sample-t sp-sample-t sp-time-t sp-sample-t*)
  (sp-state-variable-filter-hp out in in-count cutoff q-factor state)
  (void sp-sample-t* sp-sample-t* sp-sample-t sp-sample-t sp-time-t sp-sample-t*)
  (sp-state-variable-filter-bp out in in-count cutoff q-factor state)
  (void sp-sample-t* sp-sample-t* sp-sample-t sp-sample-t sp-time-t sp-sample-t*)
  (sp-state-variable-filter-br out in in-count cutoff q-factor state)
  (void sp-sample-t* sp-sample-t* sp-sample-t sp-sample-t sp-time-t sp-sample-t*)
  (sp-state-variable-filter-peak out in in-count cutoff q-factor state)
  (void sp-sample-t* sp-sample-t* sp-sample-t sp-sample-t sp-time-t sp-sample-t*)
  (sp-state-variable-filter-all out in in-count cutoff q-factor state)
  (void sp-sample-t* sp-sample-t* sp-sample-t sp-sample-t sp-time-t sp-sample-t*))

(pre-define
  (sp-cheap-filter-lp ...) (sp-cheap-filter sp-state-variable-filter-lp __VA_ARGS__)
  (sp-cheap-filter-hp ...) (sp-cheap-filter sp-state-variable-filter-hp __VA_ARGS__)
  (sp-cheap-filter-bp ...) (sp-cheap-filter sp-state-variable-filter-bp __VA_ARGS__)
  (sp-cheap-filter-br ...) (sp-cheap-filter sp-state-variable-filter-br __VA_ARGS__))

(pre-define (define-sp-state-variable-filter suffix transfer)
  (begin
    "samples real real pair [integer integer integer] -> state
     define a routine for a fast filter that also supports multiple filter types in one.
     state must hold two elements and is to be allocated and owned by the caller.
     cutoff is as a fraction of the sample rate between 0 and 0.5.
     uses the state-variable filter described here:
     * http://www.cytomic.com/technical-papers
     * http://www.earlevel.com/main/2016/02/21/filters-for-synths-starting-out/"
    (define ((pre-concat sp-state-variable-filter_ suffix) out in in-count cutoff q-factor state)
      (void sp-sample-t* sp-sample-t* sp-sample-t sp-sample-t sp-time-t sp-sample-t*)
      (declare
        a1 sp-sample-t
        a2 sp-sample-t
        g sp-sample-t
        ic1eq sp-sample-t
        ic2eq sp-sample-t
        i sp-time-t
        k sp-sample-t
        v0 sp-sample-t
        v1 sp-sample-t
        v2 sp-sample-t)
      (set
        ic1eq (array-get state 0)
        ic2eq (array-get state 1)
        g (tan (* M_PI cutoff))
        k (- 2 (* 2 q-factor))
        a1 (/ 1 (+ 1 (* g (+ g k))))
        a2 (* g a1))
      (for ((set i 0) (< i in-count) (set i (+ 1 i)))
        (set
          v0 (array-get in i)
          v1 (+ (* a1 ic1eq) (* a2 (- v0 ic2eq)))
          v2 (+ ic2eq (* g v1))
          ic1eq (- (* 2 v1) ic1eq)
          ic2eq (- (* 2 v2) ic2eq)
          (array-get out i) transfer))
      (set (array-get state 0) ic1eq (array-get state 1) ic2eq))))

(define-sp-state-variable-filter lp v2)
(define-sp-state-variable-filter hp (- v0 (* k v1) v2))
(define-sp-state-variable-filter bp v1)
(define-sp-state-variable-filter br (- v0 (* k v1)))
(define-sp-state-variable-filter peak (+ (- (* 2 v2) v0) (* k v1)))
(define-sp-state-variable-filter all (- v0 (* 2 k v1)))

(define (sp-cheap-filter-state-new max-size passes out)
  (status-t sp-time-t sp-filter-passes-t sp-cheap-filter-state-t**)
  "one state object per pass.
   heap memory is to be freed with sp-cheap-filter-state-free but only allocated if passes is greater than one"
  status-declare
  (declare in-temp sp-sample-t* out-temp sp-sample-t*)
  (error-memory-init 3)
  (sp-malloc-type-srq 1 sp-cheap-filter-state-t out)
  (error-memory-add *out)
  (if (< 1 passes)
    (begin
      (status-require (sp-samples-new max-size &in-temp))
      (error-memory-add in-temp)
      (status-require (sp-samples-new max-size &out-temp))
      (error-memory-add out-temp))
    (set in-temp 0 out-temp 0))
  (struct-pointer-set *out in-temp in-temp out-temp out-temp)
  (memset (struct-pointer-get *out svf-state) 0 (* 2 sp-filter-passes-limit (sizeof sp-sample-t)))
  (label exit (if status-is-failure error-memory-free) status-return))

(define (sp-cheap-filter-state-free a) (void sp-cheap-filter-state-t*)
  (free a:in-temp)
  (free a:out-temp))

(define (sp-cheap-filter filter in in-size cutoff passes q-factor state out)
  (void sp-state-variable-filter-t sp-sample-t* sp-time-t sp-sample-t sp-time-t sp-sample-t sp-cheap-filter-state-t* sp-sample-t*)
  "the sph-sp default fast filter. caller has to manage the state object with
   sp-cheap-filter-state-new and sp-cheap-filter-state-free.
   uses separate svf-state values for multiple passes as it is like filters in series"
  (declare in-swap sp-sample-t* in-temp sp-sample-t* out-temp sp-sample-t* i sp-time-t)
  (if (= 1 passes) (filter out in in-size cutoff q-factor state:svf-state)
    (begin
      (set in-temp state:in-temp out-temp state:out-temp)
      (filter in-temp in in-size cutoff q-factor state:svf-state)
      (for ((set i 1) (< i (- passes 1)) (set+ i 1))
        (filter out-temp in-temp in-size cutoff q-factor (+ state:svf-state (* i 2)))
        (sp-samples-zero in-temp in-size)
        (set in-swap in-temp in-temp out-temp out-temp in-swap))
      (filter out in-temp in-size cutoff q-factor (+ state:svf-state (* i 2))))))