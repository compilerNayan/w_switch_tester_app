#ifndef SWITCHDEVICE_H
#define SWITCHDEVICE_H

#include <StandardDefines.h>
#include "ISwitchDevice.h"
#include "SwitchState.h"
#include "IPhysicalSwitchReader.h"
#include "IRelayController.h"
#include "ILogger.h"
#include "Tag.h"
#include "../repository/SwitchRepository.h"
#include "../entity/Switch.h"
#include "../dto/SwitchResponseDto.h"

class SwitchDevice : public ISwitchDevice {
    Private Int id;
    Private Int relayPin;
    Private Int switchPin;
    Private SwitchState virtualState;
    Private SwitchState relayState;

    /* @Autowired */
    Private IPhysicalSwitchReaderPtr physicalSwitchReader;

    /* @Autowired */
    Private IRelayControllerPtr relayController;

    /* @Autowired */
    Private ILoggerPtr logger;

    /* @Autowired */
    Private SwitchRepositoryPtr switchRepository;

    /**
     * @brief Constructor with id, relayPin, and switchPin parameters
     * @param id The switch ID
     * @param relayPin The relay GPIO pin number
     * @param switchPin The physical switch GPIO pin number
     */
    Public SwitchDevice(CInt id, CInt relayPin, CInt switchPin) : id(id), relayPin(relayPin), switchPin(switchPin), virtualState(SwitchState::Off), relayState(SwitchState::Off) {
        // Initialize virtualState from repository
        optional<Switch> switchEntity = switchRepository->FindById(id);
        if (switchEntity.has_value() && switchEntity.value().GetVirtualState().has_value()) {
            SwitchState savedVirtualState = switchEntity.value().GetVirtualState().value();
            virtualState = savedVirtualState;
        }
    
        // Initialize relayState from relay controller
        relayState = relayController->GetState(relayPin);       

        RefreshRelayState();        
    }

    Public Virtual ~SwitchDevice() = default;

    Public Virtual SwitchState TurnOn() override {
        SwitchState physicalState = ReadPhysicalState();
        // To achieve actual ON: virtual and physical must match (so set virtual = physical)
        virtualState = physicalState;

        Var switchEntity = GetSwitchEntity();
        switchRepository->Update(switchEntity);

        RefreshRelayState();
        logger->Info(Tag::Untagged, GetOperationLogMessage("on"));

        return relayState;
    }

    Public Virtual SwitchState TurnOff() override {
        SwitchState physicalState = ReadPhysicalState();
        // To achieve actual OFF: virtual and physical must differ
        virtualState = (physicalState == SwitchState::On) ? SwitchState::Off : SwitchState::On;

        Var switchEntity = GetSwitchEntity();
        switchRepository->Update(switchEntity);

        RefreshRelayState();
        logger->Info(Tag::Untagged, GetOperationLogMessage("off"));

        return relayState;
    }

    Public Virtual SwitchState Toggle() override {
        SwitchState currentState = GetState();
        SwitchState finalState = (currentState == SwitchState::On) ? TurnOff() : TurnOn();
        logger->Info(Tag::Untagged, GetOperationLogMessage("toggle"));
        return finalState;
    }

    Public Virtual SwitchState GetState() override {
        // Read physical state from pin
        SwitchState physicalState = ReadPhysicalState();
        
        // Actual state is ON if virtual and physical states match
        // Actual state is OFF if virtual and physical states differ
        SwitchState actualState = (virtualState == physicalState) ? SwitchState::On : SwitchState::Off;
        
        //logger->Info(Tag::Untagged, GetStateLogMessage(actualState, virtualState, physicalState, switchPin));
        
        return actualState;
    }

    Public Virtual Int GetId() const override {
        return id;
    }

    Public Virtual SwitchState GetRelayState() const override {
        return relayState;
    }

    Public Virtual SwitchState GetPhysicalSwitchState() override {
        return ReadPhysicalState();
    }

    Public Virtual SwitchState GetVirtualState() const override {
        return virtualState;
    }

    Public Virtual SwitchResponseDto GetSwitchDetails() override {
        return SwitchResponseDto(
            GetId(),
            GetVirtualState(),
            GetPhysicalSwitchState(),
            GetRelayState()
        );
    }

    Public Virtual Void Refresh() override {
        // Get current actual state
        SwitchState currentState = GetState();
        
        // If actual state doesn't match relay state, update relay state
        if (currentState != relayState) {
            relayState = currentState;
            RefreshRelayState();
            
            logger->Info(Tag::Untagged, GetRefreshLogMessage(relayState, currentState));
        }
    }

    Private SwitchState ReadPhysicalState() {
        return physicalSwitchReader->ReadPhysicalState(switchPin);
    }

    /**
     * @brief Convert SwitchState to string representation
     * @param state The switch state to convert
     * @return "ON" if state is On, "OFF" if state is Off
     */
    Private Static inline StdString StateToString(SwitchState state) {
        return (state == SwitchState::On) ? "ON" : "OFF";
    }

    /**
     * @brief Generate log message for switch operations (on, off, toggle)
     * Actual = ON when virtual and physical match (both ON or both OFF); actual = OFF when they differ.
     * Relay is driven to actual state, so "actual: ON" means relay is ON.
     */
    Private inline StdString GetOperationLogMessage(CStdString operation) {
        SwitchState physicalState = ReadPhysicalState();
        SwitchState actualState = (virtualState == physicalState) ? SwitchState::On : SwitchState::Off;

        if (operation == "toggle") {
            return "Toggled switch to " + StateToString(actualState) + ", relay: " + StateToString(relayState);
        } else {
            StdString operationText = (operation == "on") ? "Turned on" : "Turned off";
            return operationText + " switch (virtual: " + StateToString(virtualState) +
                   ", physical: " + StateToString(physicalState) +
                   ", actual: " + StateToString(actualState) +
                   ", relay: " + StateToString(relayState) + ")";
        }
    }

    /**
     * @brief Generate log message for GetState operation
     * @param actualState The actual state
     * @param virtualState The virtual state
     * @param physicalState The physical state
     * @param pin The pin number
     * @return Formatted log message string
     */
    Private Static inline StdString GetStateLogMessage(SwitchState actualState, SwitchState virtualState, SwitchState physicalState, Int pin) {
        return "Get switch state: " + 
               StateToString(actualState) + 
               " (virtual: " + 
               StateToString(virtualState) + 
               ", physical: " + 
               StateToString(physicalState) + 
               ", pin: " + std::to_string(pin) + ")";
    }

    /**
     * @brief Generate log message for Refresh operation
     * @param relayState The new relay state
     * @param currentState The previous state
     * @return Formatted log message string
     */
    Private Static inline StdString GetRefreshLogMessage(SwitchState relayState, SwitchState currentState) {
        return "Refreshed relay state to " + 
               StateToString(relayState) + 
               " (was: " + 
               StateToString(currentState) + ")";
    }

    /**
     * @brief Refresh the relay state based on virtual and physical states
     * Detects the actual state (virtual == physical ? On : Off) and sets relay accordingly
     */
    Private Void RefreshRelayState() {
        // Read current physical state
        SwitchState physicalState = ReadPhysicalState();
        
        // Actual state is ON if virtual and physical states match
        // Actual state is OFF if virtual and physical states differ
        SwitchState actualState = (virtualState == physicalState) ? SwitchState::On : SwitchState::Off;
        
        // Set relay to match the actual state
        relayController->SetState(relayPin, actualState);
        relayState = actualState;
    }

    /**
     * @brief Create a Switch entity with current id and virtualState
     * @return Switch entity with current id and virtualState
     */
    Private Switch GetSwitchEntity() {
        Switch switchEntity;
        switchEntity.SetId(optional<int>(id));
        switchEntity.SetVirtualState(optional<SwitchState>(virtualState));
        return switchEntity;
    }

};

#endif // SWITCHDEVICE_H

