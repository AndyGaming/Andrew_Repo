# L-System.py
# This tool is made for Maya 2016.
# It uses turtle graphics and l-system algorithm to generate trees in Maya.

import pymel.core as pm
import random as rd

class Tree:

    # Cylinder attrs

    r = 0.5
    h = 6
    sx = 6
    sy = 1

    # Variables for user input

    rule_1 = 'F=FF-[-F+F+F]+[+F-F-F]'
    number_of_rules = 4
    rules = []
    axiom = ''
    iterations = 0
    angle = 0

    # Boolean values

    is_flat = False
    can_draw = True

    def __init__(self):
        self.make_ui()

    def make_cylinder(self):
        pm.polyCylinder(r=self.r, h=self.h, sx=self.sx, sy=self.sy)
        pm.move(0, 3, 0, 'pCylinder1.scalePivot', 'pCylinder1.rotatePivot', r=True,)

    def delete_all(self):
        pm.select(all=True)
        pm.delete()

    def combine(self):

        # Combines all cylinders and delete all history

        pm.select(all=True)
        pm.polyUnite(muv=1)
        pm.delete(ch=True)

    def get_variables(self):

        # Obtain user input and store them in variables to use later

        self.iterations = pm.intField('IterationsField', q=True, v=True)
        self.angle = pm.floatField('AngleField', q=True, v=True)
        self.axiom = pm.textField('AxiomField', q=True, tx=True)

        # Get all rules from user input

        self.rules = []
        for i in range(1, self.number_of_rules + 1):
            temp = pm.textField('RuleField{0}'.format(i), q=True,
                                tx=True)
            if temp:
                self.rules.append(temp)

        self.check_input(self.iterations, self.axiom, self.rules)

    def check_input(self, itr, axiom, rules):

        # Check user input for errors
        #     Args:
        #     All args are user input obtained from get_variables

        self.can_draw = True

        if itr < 1 or itr > 5:
            pm.confirmDialog(
                t='Error',
                m='Number of iterations must be between 1 and 5',
                icn='critical',
                b='OK',
                db='OK',
                cb='OK',
                )
            self.can_draw = False

        if '=' in axiom:
            pm.confirmDialog(
                t='Error',
                m='Invalid input for axiom',
                icn='critical',
                b='OK',
                db='OK',
                cb='OK',
                )
            self.can_draw = False

        for i in range(0, len(rules)):
            if not rules[i][1] == '=':
                pm.confirmDialog(
                    t='Error',
                    m='Invalid input for rule {0}'.format(i + 1),
                    icn='critical',
                    b='OK',
                    db='OK',
                    cb='OK',
                    )
                self.can_draw = False

    def get_pos(self):

        # Get and return the selected object's world position

        posX = pm.getAttr('.translateX')
        posY = pm.getAttr('.translateY')
        posZ = pm.getAttr('.translateZ')
        pos = [posX, posY, posZ]
        return pos

    def get_rot(self):

        # Get and return the selected object's world rotation

        rotX = pm.getAttr('.rotateX')
        rotY = pm.getAttr('.rotateY')
        rotZ = pm.getAttr('.rotateZ')
        rot = [rotX, rotY, rotZ]
        return rot

    def rand(self, angle):

        # Randomize angle
        #     Args:
        #     angle(float): This is the angle from user input
        #     Returns:
        #     (float): A randomized angle

        rand_angle = rd.uniform(angle / 3 * 2, angle)
        return rand_angle

    def preset(self, num):

        # Overwite textField values with pre-set values based on user's choice
        #     Args:
        #     num(int): Determines which set of values to use

        t_axiom = ''
        t_rule1 = ''
        t_rule2 = ''
        t_itr = 0
        t_angle = 0

        if num == 1:
            t_axiom = 'F'
            t_rule1 = 'F=FF-[-F+F+F]+[+F-F-F]'
            t_itr = 4
            t_angle = 22
            self.is_flat = False
        elif num == 2:
            t_axiom = 'FX'
            t_rule1 = 'F=FF-[-F+F]+[+F-F]'
            t_rule2 = 'X=FF+[+F]+[-F]'
            t_itr = 4
            t_angle = 25
            self.is_flat = False
        elif num == 3:
            t_axiom = 'X'
            t_rule1 = 'X=F-[[X]+X]+F[+FX]-X'
            t_rule2 = 'F=FF'
            t_itr = 4
            t_angle = 25
            self.is_flat = False

        pm.textField('AxiomField', tx=t_axiom, edit=True)
        pm.textField('RuleField1', tx=t_rule1, edit=True)
        pm.textField('RuleField2', tx=t_rule2, edit=True)
        pm.intField('IterationsField', v=t_itr, edit=True)
        pm.floatField('AngleField', v=t_angle, edit=True)

    def set_dict(self):

        # Seperate user input rules[] to keys and values of a dictionary

        dict = {}
        rules = self.rules
        for i in range(0, len(rules)):
            dict[rules[i][0]] = (rules[i])[2:]
        return dict

    def set_str(self, axiom, rules, itr):

        # Interpret axiom and rules to make a generator.
        #     Args:
        #     axiom(str): User defined axiom, self.axiom
        #     rules(dict): Generated by set_str()
        #     itr(int): User defined iterations, self.iterations
        #     Returns:
        #     (generator): Since the result will only be used once, a generator is ideal

        if itr == 0:
            for i in axiom:
                yield i
        else:
            for i in axiom:
                if i in rules:
                    for j in self.set_str(rules[i], rules, itr - 1):
                        yield j
                else:
                    yield i

    def draw(self):

        # Use the generator to draw, each char represents different behavior. 

        self.get_variables()
        string = self.set_str(self.axiom, self.set_dict(),
                              self.iterations)
        pos_stack = []
        rot_stack = []

        if self.can_draw:
            self.make_cylinder()

            for i in string:
                if i == '+':
                    if self.is_flat:
                        pm.rotate(
                            -self.angle,
                            0,
                            0,
                            r=True,
                            os=True,
                            fo=True,
                            )
                    else:
                        pm.rotate(
                            -self.rand(self.angle),
                            self.angle,
                            0,
                            r=True,
                            os=True,
                            fo=True,
                            )
                elif i == '-':
                    if self.is_flat:
                        pm.rotate(
                            self.angle,
                            0,
                            0,
                            r=True,
                            os=True,
                            fo=True,
                            )
                    else:
                        pm.rotate(
                            self.rand(self.angle),
                            -self.angle,
                            0,
                            r=True,
                            os=True,
                            fo=True,
                            )
                elif i == '[':

                    # Save current states to stacks

                    pos_stack.append(self.get_pos())
                    rot_stack.append(self.get_rot())
                elif i == ']':

                    # Get the top of each stack and pop

                    curr_pos = pos_stack.pop()
                    curr_rot = rot_stack.pop()
                    pm.setAttr('.translateX', curr_pos[0])
                    pm.setAttr('.translateY', curr_pos[1])
                    pm.setAttr('.translateZ', curr_pos[2])
                    pm.setAttr('.rotateX', curr_rot[0])
                    pm.setAttr('.rotateY', curr_rot[1])
                    pm.setAttr('.rotateZ', curr_rot[2])
                else:
                    pm.move(0, self.h, 0, r=True, os=True, wd=True)
                    pm.duplicate()
            pm.delete()

    def make_ui(self):
        window_title = 'L-System'
        window_name = 'lSystem'
        width = 300
        height = 445
        default_radius = 2
        radius_min = 0
        raduis_max = 10

        if pm.window(window_name, exists=True):
            pm.deleteUI(window_name)

        self.window = pm.window(
            window_name,
            title=window_title,
            widthHeight=(width, height),
            mnb=False,
            mxb=False,
            rtf=True,
            sizeable=False,
            )

        pm.columnLayout()

        pm.text(l='L-System Generator', w=300, h=30, bgc=(.7, .7, .7), fn='boldLabelFont')
        pm.text(l='', h=5)

        pm.frameLayout(l='Input Rules')
        pm.rowLayout(nc=2, cw2=(50, 245), ct2=('both', 'both'))
        pm.text('AxiomText', l='Axiom:', h=30)
        pm.textField('AxiomField', h=30)
        pm.setParent('..')

        pm.rowLayout(nc=2, cw2=(50, 245), ct2=('both', 'both'))
        pm.text('RuleText1', l='Rule 1:', h=30)
        pm.textField('RuleField1', h=30)
        pm.setParent('..')

        pm.rowLayout(nc=2, cw2=(50, 245), ct2=('both', 'both'))
        pm.text('RuleText2', l='Rule 2:', h=30)
        pm.textField('RuleField2', h=30)
        pm.setParent('..')

        pm.rowLayout(nc=2, cw2=(50, 245), ct2=('both', 'both'))
        pm.text('RuleText3', l='Rule 3:', h=30)
        pm.textField('RuleField3', h=30)
        pm.setParent('..')

        pm.rowLayout(nc=2, cw2=(50, 245), ct2=('both', 'both'))
        pm.text('RuleText4', l='Rule 4:', h=30)
        pm.textField('RuleField4', h=30)
        pm.setParent('..')

        pm.rowLayout(nc=2, cw2=(60, 100), ct2=('both', 'both'))
        pm.text('Iterations', l='Iterations', h=20)
        pm.intField('IterationsField', h=20)
        pm.setParent('..')

        pm.rowLayout(nc=2, cw2=(60, 100), ct2=('both', 'both'))
        pm.text('Angle', l='Angle', h=20)
        pm.floatField('AngleField', h=20)
        pm.setParent('..')

        pm.text(l='', h=5)

        # Finish frame layout for input rules

        pm.setParent('..')

        pm.frameLayout(l='Predefined Trees')
        pm.text(l='', h=5)
        pm.radioButtonGrp(la3=['Shape 1', 'Shape 2', 'Shape 3'], nrb=3,
                          on1=pm.Callback(self.preset, 1),
                          on2=pm.Callback(self.preset, 2),
                          on3=pm.Callback(self.preset, 3))
        pm.text(l='', h=5)
        pm.setParent('..')

        pm.frameLayout(l='Select')
        pm.rowLayout(nc=3, ct3=('both', 'both', 'both'))
        pm.button(l='Create', w=97, h=30, c=pm.Callback(self.draw))
        pm.button(l='Combine', w=97, h=30, c=pm.Callback(self.combine))
        pm.button(l='Delete', w=97, h=30,
                  c=pm.Callback(self.delete_all))
        pm.setParent('..')

        # Finish frame layout for select

        pm.setParent('..')

        self.window.show()
        self.window.setWidthHeight((width, height))


# Run module

if __name__ == '__main__':
    tree = Tree()

			