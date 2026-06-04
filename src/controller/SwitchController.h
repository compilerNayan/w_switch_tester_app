#ifndef SWITCHCONTROLLER_H
#define SWITCHCONTROLLER_H

#include <StandardDefines.h>
#include "ISwitchController.h"
#include "../dto/SwitchResponseDto.h"
#include "ResponseEntity.h"
#include "HttpStatus.h"
#include "../service/ISwitchService.h"

/* @RestController */
/* @RequestMapping("/switch") */
class SwitchController final : public ISwitchController {
    /* @Autowired */
    Private ISwitchServicePtr switchService;

    Public SwitchController() = default;

    Public Virtual ~SwitchController() = default;

    /* @PutMapping("/{id}/on") */
    Public Virtual ResponseEntity<SwitchResponseDto> TurnOnSwitch(/* @PathVariable("id") */ Int id) override {
        optional<SwitchResponseDto> result = switchService->TurnOnSwitch(id);
        if (!result.has_value()) {
            return ResponseEntity<SwitchResponseDto>::NotFound(SwitchResponseDto());
        }
        return ResponseEntity<SwitchResponseDto>::Ok(result.value());
    }

    /* @PutMapping("/{id}/off") */
    Public Virtual ResponseEntity<SwitchResponseDto> TurnOffSwitch(/* @PathVariable("id") */ Int id) override {
        optional<SwitchResponseDto> result = switchService->TurnOffSwitch(id);
        if (!result.has_value()) {
            return ResponseEntity<SwitchResponseDto>::NotFound(SwitchResponseDto());
        }
        return ResponseEntity<SwitchResponseDto>::Ok(result.value());
    }

    /* @PutMapping("/{id}/toggle") */
    Public Virtual ResponseEntity<SwitchResponseDto> ToggleSwitch(/* @PathVariable("id") */ Int id) override {
        optional<SwitchResponseDto> result = switchService->ToggleSwitch(id);
        if (!result.has_value()) {
            return ResponseEntity<SwitchResponseDto>::NotFound(SwitchResponseDto());
        }
        return ResponseEntity<SwitchResponseDto>::Ok(result.value());
    }

    /* @GetMapping("/{id}") */
    Public Virtual ResponseEntity<SwitchResponseDto> GetSwitchStateById(/* @PathVariable("id") */ Int id) override {
        optional<SwitchResponseDto> result = switchService->GetSwitchStateById(id);
        if (!result.has_value()) {
            return ResponseEntity<SwitchResponseDto>::NotFound(SwitchResponseDto());
        }
        return ResponseEntity<SwitchResponseDto>::Ok(result.value());
    }

    /* @GetMapping */
    Public Virtual ResponseEntity<StdVector<SwitchResponseDto>> GetAllSwitchState() override {
        StdVector<SwitchResponseDto> list = switchService->GetAllSwitchState();
        return ResponseEntity<StdVector<SwitchResponseDto>>::Ok(list);
    }
};

#endif // SWITCHCONTROLLER_H

