# ACPI tables are not yet authored for stallion (Tensor G4).
# Placeholder kept commented-out -- bring-up relies on the SimpleFb console
# and runtime hardware values from MemoryMapLib, not ACPI.
#FILE FREEFORM = 7E374E25-8E01-4FEE-87F2-390C23C606CD {
#  SECTION RAW = ZumaPro/APIC.aml
#  SECTION RAW = ZumaPro/GTDT.aml
#  SECTION RAW = stallion/DSDT.aml
#  SECTION UI = "AcpiTables"
#}
