#ifndef __TAG_H__
#define __TAG_H__

#define TAG_4F "\x4F" // Application Identifier (AID)

#define TAG_50 "\x50" // Application Label
#define TAG_57 "\x57" // Track 2 Equivalent Data
#define TAG_5A "\x5A" // Application Primary Account Number (PAN)

#define TAG_71 "\x71" // Issuer Script 1 execute before GAC
#define TAG_72 "\x72" // Issuer Script execute after GAC

#define TAG_82 "\x82" // Application Interchange Profile
#define TAG_84 "\x84" // Dedicated File (DF) Name
#define TAG_89 "\x89" // Authorisation Code
#define TAG_8A "\x8A" // Authorisation Response Code
#define TAG_8E "\x8E" // Cardholder Verification Method (CVM) Lis

#define TAG_91 "\x91" // Issuer Authentication Data
#define TAG_95 "\x95" // Terminal Verification Results
#define TAG_97 "\x97" // Transaction Certificate Data Object List
#define TAG_99 "\x99" // Transaction Personal Identification Number (PIN) Data
#define TAG_9A "\x9A" // Transaction Date
#define TAG_9B "\x9B" // Transaction Status Information
#define TAG_9C "\x9C" // Transaction Type

#define TAG_5F20 "\x5F\x20" // Cardholder Name
#define TAG_5F24 "\x5F\x24" // Application Expiration Date
#define TAG_5F25 "\x5F\x25" // Application Effective Date
#define TAG_5F2A "\x5F\x2A" // Transaction Currency Code
#define TAG_5F30 "\x5F\x30" // Service Code
#define TAG_5F34 "\x5F\x34" // Application Primary Account Number (PAN) Sequence Number
#define TAG_5F36 "\x5F\x36" // Transaction Currency Exponent

#define TAG_9F01 "\x9F\x01" // Acquirer Identifier
#define TAG_9F02 "\x9F\x02" // Amount
#define TAG_9F03 "\x9F\x03" // Other Amount
#define TAG_9F06 "\x9F\x06" // Application Identifier(AID)
#define TAG_9F07 "\x9F\x07" // Application Usage Control
#define TAG_9F08 "\x9F\x08" // Application Version Number(card)
#define TAG_9F09 "\x9F\x09" // Application Version Number(reader)
#define TAG_9F0D "\x9F\x0D" // Issuer Action Code â€?Default
#define TAG_9F0E "\x9F\x0E" // Issuer Action Code â€?Denial
#define TAG_9F0F "\x9F\x0F" // Issuer Action Code â€?Online
#define TAG_9F10 "\x9F\x10" // Issuer Application Data
#define TAG_9F12 "\x9F\x12" // Application Preferred Name
#define TAG_9F15 "\x9F\x15" // Merchant Category Code
#define TAG_9F16 "\x9F\x16" // Merchant Identifier
#define TAG_9F17 "\x9F\x17" // PIN Retry Times
#define TAG_9F1A "\x9F\x1A" // Terminal Country Code
#define TAG_9F1B "\x9F\x1B" // Terminal Floor Limit
#define TAG_9F1C "\x9F\x1C" // Terminal Identification
#define TAG_9F1D "\x9F\x1D" // Terminal Risk Management Data
#define TAG_9F1E "\x9F\x1E" // Interface Device(IFD) Serial Number
#define TAG_9F21 "\x9F\x21" // Transaction Time
#define TAG_9F22 "\x9F\x22" // CA Public Key Index
#define TAG_9F26 "\x9F\x26" // Application Cryptogram
#define TAG_9F27 "\x9F\x27" // Cryptogram Information Data
#define TAG_9F33 "\x9F\x33" // Terminal Capabilities
#define TAG_9F34 "\x9F\x34" // Cardholder Verification Method (CVM) Results
#define TAG_9F35 "\x9F\x35" // Terminal Type
#define TAG_9F36 "\x9F\x36" // Application Transaction Counter (ATC)
#define TAG_9F37 "\x9F\x37" // Unpredictable Number
#define TAG_9F39 "\x9F\x39" // Point-of-Service (POS) Entry Mode
#define TAG_9F3A "\x9F\x3A" // Amount Reference Currency
#define TAG_9F3C "\x9F\x3C" // Transaction Reference Currency Code
#define TAG_9F3D "\x9F\x3D" // Transaction Reference Currency Exponent
#define TAG_9F40 "\x9F\x40" // Additional Terminal Capabilities
#define TAG_9F41 "\x9F\x41" // Transaction Sequence Counter
#define TAG_9F4E "\x9F\x4E" // Merchant Name and Location
#define TAG_9F53 "\x9F\x53" // Transaction Category Code
#define TAG_9F61 "\x9F\x61" // Cardholder Cert ID
#define TAG_9F62 "\x9F\x62" // Cardholder Cert Type
#define TAG_9F66 "\x9F\x66" // Terminal Transaction Qualifiers
#define TAG_9F7B "\x9F\x7B" // EC TTL value

#define TAG_DF01 "\xDF\x01" // Application Selection Flag
#define TAG_DF02 "\xDF\x02" // Module Of Key
#define TAG_DF03 "\xDF\x03" // Check Sum Of Key
#define TAG_DF04 "\xDF\x04" // Exponent Of Key
#define TAG_DF05 "\xDF\x05" // Expiration Date
#define TAG_DF06 "\xDF\x06" // Hash Arithmetic Indicator
#define TAG_DF07 "\xDF\x07" // RSA Arithmetic Indicator
#define TAG_DF11 "\xDF\x11" // Terminal Action Code - Default
#define TAG_DF12 "\xDF\x12" // Terminal Action Code - Online
#define TAG_DF13 "\xDF\x13" // Terminal Action Code - Denial
#define TAG_DF14 "\xDF\x14" // Terminal Default DDOL
#define TAG_DF15 "\xDF\x15" // Threshold
#define TAG_DF16 "\xDF\x16" // Max Target Percent
#define TAG_DF17 "\xDF\x17" // Target Percent
#define TAG_DF18 "\xDF\x18" // Online PIN Flag
#define TAG_DF19 "\xDF\x19" // Reader Floor Limit
#define TAG_DF20 "\xDF\x20" // Reader Offline Floor Limit
#define TAG_DF21 "\xDF\x21" // Reader CVM Floor Limit
#define TAG_DF31 "\xDF\x31" // aid dynamic tags
#define TAG_DF51 "\xDF\x51" // Issuer Script Result

#define TAG_FF9001 "\xFF\x90\x01" // Transaction Detect Card Timeout
#define TAG_FF9002 "\xFF\x90\x02" // Transaction Online Timeout
#define TAG_FF9003 "\xFF\x90\x03" // Transaction Input PIN Timeout
#define TAG_FF9004 "\xFF\x90\x04" // Transaction Check ID Timeout
#define TAG_FF9005 "\xFF\x90\x05" // Merchant Force Online
#define TAG_FF9006 "\xFF\x90\x06" // PIN Type
#define TAG_FF9007 "\xFF\x90\x07" // Notice Type
#define TAG_FF9008 "\xFF\x90\x08" // Mag Card Track Encrypted Flag
#define TAG_FF9009 "\xFF\x90\x09" // Mag Card Online PIN Flag
#define TAG_FF900A "\xFF\x90\x0A" // Transaction Card Type
#define TAG_FF900B "\xFF\x90\x0B" // Read Card Log Flag
#define TAG_FF9021 "\xFF\x90\x21" // Key Template
#define TAG_FF9022 "\xFF\x90\x22" // Key Group Index
#define TAG_FF9023 "\xFF\x90\x23" // Key Index
#define TAG_FF9024 "\xFF\x90\x24" // Key Usage
#define TAG_FF9025 "\xFF\x90\x25" // Key KSN
#define TAG_FF9026 "\xFF\x90\x26" // Key data
#define TAG_FF9027 "\xFF\x90\x27" // Key Check Value
#define TAG_FFA001 "\xFF\xA0\x01" // Mag Card Track 1
#define TAG_FFA002 "\xFF\xA0\x02" // Mag Card Track 1 Encrypted
#define TAG_FFA003 "\xFF\xA0\x03" // Mag Card Track 2
#define TAG_FFA004 "\xFF\xA0\x04" // Mag Card Track 2 Encrypted
#define TAG_FFA005 "\xFF\xA0\x05" // Mag Card Track 3
#define TAG_FFA006 "\xFF\xA0\x06" // Mag Card Track 3 Encrypted
#define TAG_FFA007 "\xFF\xA0\x07" // Online PIN block
#define TAG_FFA008 "\xFF\xA0\x08" // Clear Message
#define TAG_FFA009 "\xFF\xA0\x09" // Encrypted Message
#define TAG_FFA00A "\xFF\xA0\x0A" // MAC
#define TAG_FFA00B "\xFF\xA0\x0B" // MAC Calculate algorithm
#define TAG_FFF001 "\xFF\xF0\x01" // Command Code
#define TAG_FFF002 "\xFF\xF0\x02" // Parameters
#define TAG_FFF003 "\xFF\xF0\x03" // Return Code
#define TAG_FFF004 "\xFF\xF0\x04" // Return Value
#define TAG_FFF005 "\xFF\xF0\x05" // Transaction Result

#define TAG_7F10 "\x7F\x10" //
#define TAG_7F11 "\x7F\x11" //


#endif

