/**
 * @file
 * BLE service
 *
 * @authors Crownstone Team, Christopher Mason.
 * @copyright Crownstone B.V.
 * @date Apr 23, 2015
 * License: LGPLv3+, Apache License 2.0, and/or MIT (triple-licensed)
 */
#pragma once

#include <ble/cs_Stack.h>
#include <ble/cs_Characteristic.h>

#include <common/cs_Tuple.h>
#include <cfg/cs_Strings.h>
#include <util/cs_BleError.h>
#include <drivers/cs_Timer.h>


/** General BLE name service
 *
 * All functionality that is just general BLE functionality is encapsulated in the BLEpp namespace.
 */
class Nrf51822BluetoothStack;
class CharacteristicBase;

/** Service as defined in the GATT Specification.
 */
class Service {

public:
	static const char* defaultServiceName; //! "Generic Service"

	// might want to change this to a linked list or something that
	// we can loop over but doesn't allocate more space than needed
	typedef tuple<CharacteristicBase*> Characteristics_t;

protected:

	Nrf51822BluetoothStack*  _stack;
	UUID                     _uuid;
	std::string              _name;
	bool                     _primary;
	uint16_t                 _service_handle; //! provided by stack.
	bool                     _started;

	// per BLE definition, there is no maximum number of characteristics per
	// service. it is limited by the memory available to the GATT table over
	// all services/characteristics
	//! Currently maximum number of characteristics per service
//	static const uint8_t MAX_CHARACTERISTICS = 10;

	//! List of characteristics
//	fixed_tuple<CharacteristicBase*, MAX_CHARACTERISTICS> _characteristics;
	Characteristics_t _characteristics;

public:
	Service() :
		_stack(NULL),
		_name(""),
		_primary(true),
		_service_handle(BLE_CONN_HANDLE_INVALID),
		_started(false)
//		,_appTimerId(NULL)
{
		//! Should be done in the service, if it wants a timer
//#if (NORDIC_SDK_VERSION >= 11)
//		_appTimerData = { {0} };
//		_appTimerId = &_appTimerData;
//#endif
}

	/** Default empty destructor
	 *
	 * We don't currently delete our characteristics as we don't really support dynamic service destruction.
	 * If we wanted to allow services to be removed at runtime, we would need to, amongst many other things,
	 * keep track of whether we allocated the characteristic or whether it was passed into us.
	 */
	virtual ~Service() {
	}

	Service& setName(const char * const name) {
		_name = name;
		return *this;
	}

	Service& setUUID(const UUID& uuid) {
		if (_started) BLE_THROW("Already started.");

		_uuid = uuid;
		return *this;
	}

	Service& setIsPrimary(bool isPrimary) {
		if (_started) BLE_THROW("Already started.");

		_primary = isPrimary;
		return *this;
	}

	Nrf51822BluetoothStack* getStack() {
		return _stack;
	}

	UUID& getUUID() {
		return _uuid;
	}

	uint16_t getHandle() {
		return _service_handle;
	}

	//! internal:

	virtual void createCharacteristics() = 0;

//	virtual void tick() {};
//	static void staticTick(Service* ptr) {
//		ptr->tick();
//	}


	virtual void init(Nrf51822BluetoothStack* stack);
	virtual void stopAdvertising() {};

	virtual void on_ble_event(ble_evt_t * p_ble_evt);

	virtual void on_connect(uint16_t conn_handle, ble_gap_evt_connected_t& gap_evt);  //! FIXME NRFAPI  friend??

	virtual void on_disconnect(uint16_t conn_handle, ble_gap_evt_disconnected_t& gap_evt);  //! FIXME NRFAPI

	virtual bool on_write(ble_gatts_evt_write_t& write_evt, uint16_t value_handle);  //! FIXME NRFAPI

	virtual void onTxComplete(ble_common_evt_t * p_ble_evt);

	/** Get list of characteristics
	 *
	 * @return list of characteristics
	 */
	virtual Characteristics_t & getCharacteristics() {
		return _characteristics;
	}

	/** Add a single characteristic to the list
	 * @characteristic Characteristic to add
	 */
	virtual Service& addCharacteristic(CharacteristicBase* characteristic) {
//		if (_characteristics.size() == MAX_CHARACTERISTICS) {
//			BLE_THROW(MSG_BLE_CHAR_TOO_MANY);
//		}
		_characteristics.push_back(characteristic);

		return *this;
	}

	virtual Service& removeCharacteristic(CharacteristicBase* characteristic);

	Service& addCharacteristicsDone() {
		_characteristics.shrink_to_fit();
		return *this;
	}

	void setPinEncrypted(bool encrypted);
	void setAesEncrypted(bool encrypted);

};
