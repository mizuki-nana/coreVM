#ifndef COREVM_DYOBJ_FLAGS_H_
#define COREVM_DYOBJ_FLAGS_H_


namespace corevm {


namespace dyobj {


/* Dynamic object flags are represented by 32 bits unsigned integers. Please
 * refer to the definition of `corevm::dyobj::flag`.
 */
enum flags {
  // Bits that pertain to the lifespan of objects.
  IS_GARBAGE_COLLECTIBLE = 1,
  IS_WEAK_REF = 2,
  IS_DELETABLE = 3,
  // Bits that pertain to the scope of objects.
  IS_GLOBAL_OBJ = 4,
  IS_SUPPLIED_OBJ = 5,
  IS_VISIBLE_TO_USER = 6,
  // Bits that pertain to the various attributes of objects.
  IS_CALLABLE = 7,
  IS_MUTABLE = 8,
  // THE REMAINING BITS ARE RESERVED FOR FURTHER USE.
  LAST_PLACEHOLDER = 32,
};


}; /* end namespace dyobj */


}; /* end namespace corevm */


#endif /* COREVM_DYOBJ_FLAGS_H_ */
